#include "light.h"

Light::Light(GLuint iBindPoint, bool iIsDynamic)
: UniformBuffer(iBindPoint, iIsDynamic) {
    data.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    data.diffuse = vec4(1.0f);
    data.specular = vec4(0.5f);
}

Light::~Light() { }

const void *Light::getData() const {
    return (const void *) &data;
}

GLuint Light::getDataSize() const {
    return (GLuint) sizeof(data);
}
