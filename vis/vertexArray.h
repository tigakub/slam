#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "usedsa.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"

using namespace std;
using namespace glm;

class VertexArray {
    protected:
        GLuint vao;

    public:
        VertexArray();
        virtual ~VertexArray();

        virtual void init(ElementBufferBase & iEBuf, VertexBufferBase & iVBuf);
        virtual void bind();
        virtual void unbind();
};

#endif
