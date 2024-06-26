#ifndef __LIGHT_H__
#define __LIGHT_H__

// #include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderData.h"
#include "uniformBuffer.h"

class Light: public UniformBuffer {
    protected:
        LightData data;

    public:
        Light(GLuint iBindPoint, bool iIsDynamic = false);
        virtual ~Light();

        LightData &getLightData();

    protected:
        virtual void initData();
        virtual const void *getData() const;
        virtual GLsizei getDataSize() const;
};

#endif