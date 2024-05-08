#include "context.h"

Context::Context(GLuint iBindPoint, bool iIsDynamic)
: UniformBuffer(iBindPoint, iIsDynamic),
  transformStack(),
  data() {
    transformStack.emplace_back(mat4(1.0f));
}

const mat4 & Context::getTopMatrix() {
    return transformStack.back();
}

void Context::pushMatrix(const mat4 & iTransform) {
    data.modelMatrix = transformStack.back() * iTransform;
    mark();
    update();
    transformStack.push_back(data.modelMatrix);
}

void Context::popMatrix() {
    transformStack.pop_back();
    mat4 top = getTopMatrix();
    data.modelMatrix = top;
    mark();
    update();
}

void Context::initData() {
    data.modelMatrix = mat4(1.0f);
    data.tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    dirty = true;
}

void Context::update() {
    if(dirty) {
        UniformBuffer::update();
    }
}

const void * Context::getData() const {
    return (void *) & data;
}

GLsizei Context::getDataSize() const {
    return (GLsizei) sizeof(data);
}
