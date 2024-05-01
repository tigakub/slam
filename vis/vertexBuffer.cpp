#include "vertexBuffer.h"

VertexBufferBase::VertexBufferBase(bool iIsDynamic)
: vbo(0), isDynamic(iIsDynamic), dirty(false) { }

VertexBufferBase::~VertexBufferBase() {
}

void VertexBufferBase::init() {
    if(getData() && getDataSize()) {
        dirty = true;
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

void VertexBufferBase::cleanUp() {
    unbind();
    if(vbo) glDeleteBuffers(1, &vbo);
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
