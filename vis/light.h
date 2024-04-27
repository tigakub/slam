#ifndef __LIGHT_H__
#define __LIGHT_H__

// #include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderData.h"

class Light {
    protected:
        LightData data;
        unsigned int ubo;

    public:
        Light();

        bool init();
};

#endif