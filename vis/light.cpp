#include "light.h"

Light::Light()
: data() {
    data.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    data.diffuse = vec4(1.0f);
    data.specular = vec4(0.5f);
}

bool Light::init() {
    glCreateBuffers(1, (GLuint *) &data);
    glNamedBufferStorage(ubo, sizeof(LightData), &data, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, UB_LIGHT, ubo);

    return true;
}