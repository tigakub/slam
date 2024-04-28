#include "vertexBuffer.h"

VertexBufferBase::VertexBufferBase(bool iIsDynamic)
: isDynamic(iIsDynamic), dirty(false), vbo(0) { }

VertexBufferBase::~VertexBufferBase() {
    unbind();
    if(vbo) glDeleteBuffers(1, &vbo);
}

void VertexBufferBase::init() {
    if(getData() && getDataSize()) {
        #ifdef USEDSA
            glCreateBuffers(1, &vbo);
            glNamedBufferStorage(vbo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW_BIT : 0);
        #else
            glGenBuffers(1, &vbo);
            bind();
            glBufferData(GL_ARRAY_BUFFER, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        #endif
    }
}

#ifdef USEDSA
void VertexBufferBase::attachToVAO(GLuint iVao, GLuint iBindPoint, GLint iOffset) {
    glVertexArrayVertexBuffer(iVao, iBindPoint, getData(), &iOffset, getVertexSize());
}
#endif

void VertexBufferBase::update() {
    if(dirty && getData() && getDataSize()) {
        #ifdef USDSA
            glNamedBufferSubData(vbo, 0, getDataSize(), getData());
        #else
            bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, getDataSize(), getData());
            unbind();
        #endif
    }
}

void VertexBufferBase::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBufferBase::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
