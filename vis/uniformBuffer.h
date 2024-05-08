#ifndef __UNIFORMBUFFER_H__
#define __UNIFORMBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "usedsa.h"

using namespace std;

class UniformBuffer {
    protected:
        GLuint ubo;
        GLuint bindPoint;
        bool isDynamic;
        bool isNew;
        bool dirty;

    public:
        UniformBuffer(GLuint iBindPoint, bool iIsDynamic = false);
        virtual ~UniformBuffer();

        // virtual bool init(GLuint iBindPoint);
        virtual void mark();
        virtual void unmark();
        virtual void update();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual void initData() = 0;
        virtual const void *getData() const = 0;
        virtual GLsizei getDataSize() const = 0;
};

#endif
