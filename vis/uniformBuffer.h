#ifndef __UNIFORMBUFFER_H__
#define __UNIFORMBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "usedsa.h"

class UniformBuffer {
    protected:
        GLuint ubo;
        GLuint bindPoint;
        bool isDynamic;
        bool dirty;

    public:
        UniformBuffer(GLuint iBindPoint = 0, bool iIsDynamic = false);
        virtual ~UniformBuffer();

        virtual bool init();
        virtual void cleanUp();
        virtual void update();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLuint getDataSize() const = 0;
};

#endif
