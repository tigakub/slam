#include "vertexBuffer.h"

VertexBufferBase::VertexBufferBase(bool iIsDynamic)
: vbo(0), isDynamic(iIsDynamic), isNew(true), dirty(false) { }

VertexBufferBase::~VertexBufferBase() { 
    unbind();
    if(vbo) glDeleteBuffers(1, &vbo);
}

/*
void VertexBufferBase::init() {
    if(getData() && getDataSize()) {
        dirty = false;
        #ifdef USEDSA
            glCreateBuffers(1, &vbo);
            glNamedBufferStorage(vbo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW_BIT : 0);
        #else
            glGenBuffers(1, &vbo);
            bind();
            GLsizei dataSize = getDataSize();
            const void * data = getData();
            glBufferData(GL_ARRAY_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        #endif
    }
}
*/

#ifdef USEDSA
void VertexBufferBase::attachToVAO(GLuint iVao, GLuint iBindPoint, GLint iOffset) {
    glVertexArrayVertexBuffer(iVao, iBindPoint, getData(), &iOffset, getVertexSize());
}
#endif

void VertexBufferBase::update(bool iDeferUnbind) {
    if(isNew) {
        glGenBuffers(1, &vbo);
        bind();
        GLsizei dataSize = getDataSize();
        const void * data = getData();
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if(!iDeferUnbind) unbind();
        isNew = false;
    } else {
        if(dirty && isDynamic && getData() && getDataSize()) {
            #ifdef USDSA
                glNamedBufferSubData(vbo, 0, getDataSize(), getData());
            #else
                bind();
                glBufferSubData(GL_ARRAY_BUFFER, 0, getDataSize(), getData());
                unbind();
            #endif
        }
    }
    dirty = false;
}

void VertexBufferBase::mark() {
    dirty = true;
}

void VertexBufferBase::unmark() {
    dirty = false;
}

void VertexBufferBase::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBufferBase::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
