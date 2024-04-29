#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "vertex.h"
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

        virtual size_t getCount() const = 0;
        virtual const BufferFormat &getBufferFormat() const = 0;

        virtual void init();
        virtual void cleanUp();
        #ifdef USEDSA
        void attachToVAO(GLuint iVao, GLuint iBindPoint, GLint iOffset);
        #endif
        virtual void update();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLsizei getDataSize() const = 0;
        virtual GLsizei getVertexSize() const = 0;
        /*
        virtual GLuint getComponentCount() const = 0;
        virtual GLenum getComponentType() const = 0;
        */
};

/*
class InterleavedVertexBuffer: public VertexBufferBase {
    friend class vertexArray;

    protected:
        vector<GLfloat> data;
        vector<VertexBufferBase *> vertexBuffers;

    public:
        InterleavedVertexBuffer(VertexBufferBase * iVBufs, int iVBufCount, bool iIsDynamic = false)
        : VertexBufferBase(iIsDynamic), data(), vertexBuffers() {
            for(int i = 0; i < iVBufCount; i++) {
                vertexBuffers.push_back(iVBufs + i);
            }
        }
rm
    protected:
        virtual const void *getData() const;
        virtual GLuint getDataSize() const;
        virtual GLsizei getVertexSize() const;
        virtual GLuint getComponentCount() const;
        virtual GLenum getComponentType() const;
};
*/

template <typename DataType, const BufferFormat & iBufferFormat>
class VertexBuffer: public VertexBufferBase {
    friend class VertexArray;

    protected:
        vector<DataType> data;

    public:
        VertexBuffer(bool iIsDynamic = false)
        : VertexBufferBase(iIsDynamic) { }

        virtual size_t getCount() const {
            return data.size();
        }

        virtual const BufferFormat &getBufferFormat() const {
            return iBufferFormat;
        }

        vector<DataType> &getVertices() {
            return data;
        }

        void addVertex(DataType iVertex) {
            data.push_back(iVertex);
        }

    protected:
        virtual const void *getData() const { return (void *) &data[0]; }
        virtual GLsizei getDataSize() const { return (GLsizei) (data.size() * sizeof(DataType)); }
        virtual GLsizei getVertexSize() const { return (GLsizei) sizeof(DataType); }
        /*
        virtual GLuint getComponentCount() const { return componentCount; }
        virtual GLenum getComponentType() const { return componentType; }
        */
};

#endif
