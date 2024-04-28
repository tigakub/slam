#include "uniformBuffer.h"

UniformBuffer::UniformBuffer(GLuint iBindPoint, bool iIsDynamic)
: bindPoint(iBindPoint), isDynamic(iIsDynamic), dirty(false), ubo(0) { }

UniformBuffer::~UniformBuffer() {
    unbind();
    if(ubo) glDeleteBuffers(1, &ubo);
}

bool UniformBuffer::init() {
    #ifdef USEDSA
        glCreateBuffers(1, (GLuint *) &data);
        glNamedBufferStorage(ubo, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_STORAGE_BIT : GL_STATIC_STORAGE_BIT);
    #else
        glGenBuffers(1, &ubo);
        bind();
        if(getData() && getDataSize()) {
            glBufferData(GL_UNIFORM_BUFFER, getDataSize(), getData(), isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        unbind();
    #endif

    return true;
}

void UniformBuffer::update() {
    if(dirty) {
        #ifdef USEDSA
            glNamedBufferSubData(ubo, 0, getDataSize(), getData());
        #else
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, getDataSize(), getData());
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
