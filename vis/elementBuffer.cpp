#include "elementBuffer.h"

ElementBufferBase::ElementBufferBase(bool iIsDynamic)
: isDynamic(iIsDynamic), dirty(false), ebo(0) { }

ElementBufferBase::~ElementBufferBase() {
    unbind();
    if(ebo) glDeleteBuffers(1, &ebo);
}

void ElementBufferBase::init() {
    #ifdef USEDSA
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW_BIT : 0);
    #else
        glGenBuffers(1, &ebo);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    #endif
}

#ifdef USEDSA
void ElementBufferBase::attachToVAO(GLuint iVao) {
    glVertexArrayElementBuffer(iVao, getData());
}
#endif

void ElementBufferBase::update() {
    if(dirty) {
        #ifdef USDSA
            glNamedBufferSubData(vbo, 0, getDataSize(), getData());
        #else
            bind();
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getDataSize(), getData());
            unbind();
        #endif
    }
}

void ElementBufferBase::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void ElementBufferBase::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


