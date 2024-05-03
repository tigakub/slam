#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory> // for shared_ptr

#include "uniformBuffer.h"
#include "camera.h"

using namespace std;
using namespace glm;

class Context: public UniformBuffer {
    protected:
        Camera & camera;
        vector<mat4> transformStack;
        ContextData data;

    public:
        Context(Camera & iCamera);

        virtual const mat4 & getTopMatrix();
        virtual void pushMatrix(const mat4 & iTransform);
        virtual void popMatrix();

        virtual void update();

    protected:
        virtual void initData();
        virtual const void *getData() const;
        virtual GLsizei getDataSize() const;
};

#endif
