#ifndef __SHADERSTORAGEBUFFER_H__
#define __SHADERSTORAGEBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "vertex.h"

#include <string>

using namespace std;
using namespace glm;

class ShaderStorageBufferBase {
    protected:
        GLuint ssbo;
        GLuint bindPoint;
        bool isDynamic;
        bool isNew;
        bool dirty;
    
    public:
        ShaderStorageBufferBase(GLuint iBindPoint, bool iIsDynamic = false);
        virtual ~ShaderStorageBufferBase();

        virtual size_t getCount() const = 0;
        virtual const BufferFormat &getBufferFormat() const = 0;

        virtual void update(bool iDeferUnbind = false);
        virtual void mark();
        virtual void unmark();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLsizei getDataSize() const = 0;
};

template <typename DataType, const BufferFormat &iBufferFormat, bool iIsFixedSize = false, GLsizei iBufferSize = 0>
class ShaderStorageBuffer : public ShaderStorageBufferBase {
    protected:
        vector<DataType> data;

    public:
        ShaderStorageBuffer(GLuint iBindPoint, bool iIsDynamic = false)
        : ShaderStorageBuffer(iBindPoint, iIsDynamic) { }

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
        virtual GLsizei getDataSize() const { return (GLsizei) (iIsFixedSize ? iBufferSize : data.size() * sizeof(DataType)); }
        virtual GLsizei getVertexSize() const { return (GLsizei) sizeof(DataType); }
};

#endif
