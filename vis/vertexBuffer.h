#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "usedsa.h"

using namespace std;
using namespace glm;

class VertexBufferBase {
    friend class VertexArray;

    protected:
        GLuint vbo;
        bool isDynamic;
        bool dirty;

    public:
        VertexBufferBase(bool iIsDynamic = false);
        virtual ~VertexBufferBase();

        virtual void init();
        #ifdef USEDSA
        void attachToVAO(GLuint iVao, GLuint iBindPoint, GLint iOffset);
        #endif
        virtual void update();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLuint getDataSize() const = 0;
        virtual GLsizei getVertexSize() const = 0;
        virtual GLuint getComponentCount() const = 0;
        virtual GLenum getComponentType() const = 0;
};

template <typename DataType, GLuint componentCount = 3, GLenum componentType = GL_FLOAT>
class VertexBuffer: public VertexBufferBase {
    friend class VertexArray;

    protected:
        vector<DataType> data;

    public:
        VertexBuffer(bool iIsDynamic = false)
        : VertexBufferBase(iIsDynamic) { }

        void addVertex(DataType iVertex) {
            data.push_back(iVertex);
        }

    protected:
        virtual const void *getData() const { return (void *) &data[0]; }
        virtual GLuint getDataSize() const { return (GLuint) data.size(); }
        virtual GLsizei getVertexSize() const { return (GLsizei) sizeof(DataType); }
        virtual GLuint getComponentCount() const { return componentCount; }
        virtual GLenum getComponentType() const { return componentType; }
};

#endif
