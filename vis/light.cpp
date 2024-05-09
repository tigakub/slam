#include "light.h"

Light::Light(GLuint iBindPoint, bool iIsDynamic)
: UniformBuffer(iBindPoint, iIsDynamic) { }

Light::~Light() { }

void Light::initData() {
    data.position = vec4(3.0f, 3.0f, 3.0f, 0.0f);
    data.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    data.diffuse = vec4(1.0f);
    data.specular = vec4(0.5f);
}

const void *Light::getData() const {
    return (const void *) &data;
}

GLsizei Light::getDataSize() const {
    return (GLsizei) sizeof(data);
}
