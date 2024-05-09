#include "uniformBuffer.h"

UniformBuffer::UniformBuffer(bool iIsDynamic)
: bindPoint(0), isDynamic(iIsDynamic), dirty(false), ubo(0) { }

UniformBuffer::~UniformBuffer() {
    unbind();
    if(ubo) glDeleteBuffers(1, &ubo);
}

bool UniformBuffer::init(GLuint iBindPoint) {
    initData();
    bindPoint = iBindPoint;
    if(getData() && getDataSize()) {
        dirty = false;
        #ifdef USEDSA
            glCreateBuffers(1, (GLuint *) &data);
            glNamedBufferStorage(ubo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_STORAGE_BIT : GL_STATIC_STORAGE_BIT);
        #else
            glGenBuffers(1, &ubo);
            bind();
            glBufferData(GL_UNIFORM_BUFFER, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            unbind();
        #endif
   }

    return true;
}

void UniformBuffer::mark() {
    dirty = true;
}

void UniformBuffer::unmark() {
    dirty = false;
}

void UniformBuffer::update() {
    if(dirty && isDynamic && getData() && getDataSize()) {
        #ifdef USEDSA
            glNamedBufferSubData(ubo, 0, getDataSize(), getData());
        #else
            bind();
            glBufferSubData(GL_UNIFORM_BUFFER, 0, getDataSize(), getData());
            unbind();
        #endif
        dirty = false;
    }
}

void UniformBuffer::bind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, ubo);

}

void UniformBuffer::unbind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, 0);
}
