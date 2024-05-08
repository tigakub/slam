#include "elementBuffer.h"

ElementBufferBase::ElementBufferBase(bool iIsDynamic)
: isDynamic(iIsDynamic), isNew(true), dirty(false), ebo(0) { }

ElementBufferBase::~ElementBufferBase() {
    unbind();
    if(ebo) glDeleteBuffers(1, &ebo);
}

/*
void ElementBufferBase::init() {
    if(getData() && getDataSize()) {
        dirty = false;
        #ifdef USEDSA
            glCreateBuffers(1, &ebo);
            glNamedBufferStorage(ebo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW_BIT : 0);
        #else
            glGenBuffers(1, &ebo);
            bind();
            GLsizei dataSize = getDataSize();
            const void * data = getData();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        #endif
    }
}
*/

#ifdef USEDSA
void ElementBufferBase::attachToVAO(GLuint iVao) {
    glVertexArrayElementBuffer(iVao, getData());
}
#endif

void ElementBufferBase::update() {
    if(isNew) {
        glGenBuffers(1, &ebo);
        bind();
        GLsizei dataSize = getDataSize();
        const void * data = getData();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        isNew = false;
    } else {
        if(dirty && isDynamic && getData() && getDataSize()) {
            #ifdef USDSA
                glNamedBufferSubData(vbo, 0, getDataSize(), getData());
            #else
                bind();
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getDataSize(), getData());
                unbind();
            #endif
        }
    }
    dirty = false;
}

void ElementBufferBase:: mark() {
    dirty = true;
}

void ElementBufferBase:: unmark() {
    dirty = false;
}

void ElementBufferBase::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void ElementBufferBase::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


