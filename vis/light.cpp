#include "light.h"

Light::Light(GLuint iBindPoint, bool iIsDynamic)
: UniformBuffer(iBindPoint, iIsDynamic) {
    data.position = vec4(2.0f, 3.0f, 5.0f, 0.0f);
    data.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    data.diffuse = vec4(1.0f);
    data.specular = vec4(0.5f);
}

Light::~Light() { }

LightData & Light::getLightData() {
    return data;
}

void Light::initData() { }

const void *Light::getData() const {
    return (const void *) &data;
}

GLsizei Light::getDataSize() const {
    return (GLsizei) sizeof(data);
}
