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
        ElementBufferBase &elementBuffer;
        vector<VertexBufferBase *> vertexBuffers;
        GLuint vao;

    public:
        VertexArray(ElementBufferBase & iEBuf, VertexBufferBase * iVBuf);
        virtual ~VertexArray();

        void addVertexBuffer(VertexBufferBase * iVBuf);

        virtual void init();
        virtual void bind();
        virtual void unbind();
};

#endif
