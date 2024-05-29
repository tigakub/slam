#include "shaderStorageBuffer.h"

ShaderStorageBufferBase::ShaderStorageBufferBase(GLuint iBindPoint, bool iIsDynamic)
: ssbo(0), bindPoint(iBindPoint), isDynamic(iIsDynamic), isNew(true), dirty(false) { }

ShaderStorageBufferBase::ShaderStorageBufferBase(ShaderStorageBufferBase && iOther)
: ssbo(iOther.ssbo), bindPoint(iOther.bindPoint), isDynamic(iOther.isDynamic), isNew(iOther.isNew), dirty(iOther.dirty) {
    iOther.ssbo = 0;
}

ShaderStorageBufferBase::~ShaderStorageBufferBase() {
    unbind();
    if(ssbo) glDeleteBuffers(1, &ssbo);
}

void ShaderStorageBufferBase::update(bool iDeferUnbind) {
    if(isNew) {
        glGenBuffers(1, &ssbo);
        bind();
        GLsizei dataSize = getDataSize();
        const void * data = getData();
        glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if(!iDeferUnbind) unbind();
        isNew = false;
    } else {
        if(dirty && isDynamic && getData() && getDataSize()) {
            bind();
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, getDataSize(), getData());
            unbind();
        }
    }
    dirty = false;
}

void ShaderStorageBufferBase::mark() {
    dirty = true;
}

void ShaderStorageBufferBase::unmark() {
    dirty = false;
}

void ShaderStorageBufferBase::bind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
}

void ShaderStorageBufferBase::unbind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
