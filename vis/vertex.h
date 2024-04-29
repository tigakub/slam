#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

#include "aabb.h"

struct VertexFormat {
    GLsizei componentCount;
    GLenum componentType;
    GLsizei byteSize;
};

struct BufferFormat {
    vector<VertexFormat> format;
    GLsizei byteStride;

    BufferFormat(): format(), byteStride(0) { }
    BufferFormat(BufferFormat && iOther)
    : format(::move(iOther.format)), byteStride(iOther.byteStride) { }

    void addFormat(GLsizei iComponentCount, GLenum iComponentType, GLsizei iByteSize) {
        format.push_back(VertexFormat(iComponentCount, iComponentType, iByteSize));
        byteStride = 0;
        for(auto &vFmt: format) {
            byteStride += vFmt.byteSize;
        }
    }

    GLsizei getByteStride() const {
        GLsizei stride = 0;
        for(auto &vFmt: format) {
            stride += vFmt.byteSize;
        }
        return stride;
    }
};

/*
struct Vertex {
    virtual void expand(AABB &ioBoundingBox) const = 0;
};
*/

struct PNCUVertex {
    static BufferFormat initBufferFormat();

    alignas(4) vec3 position;
    alignas(4) vec3 normal;
    alignas(4) vec4 color;
    alignas(4) vec2 uv;

    static BufferFormat bufferFormat;

    PNCUVertex(vec3 iPosition, vec3 iNormal, vec4 iColor, vec2 iUV);
    /*
    virtual const BufferFormat &getBufferFormat() const { 
        return bufferFormat;
    }
    */
    /*
    virtual void expand(AABB &ioBoundingBox) const {
        ioBoundingBox.expand(position);
    }
    */
};

struct PCVertex {
    static BufferFormat initBufferFormat();

    alignas(4) vec3 position;
    alignas(4) vec4 color;

    static BufferFormat bufferFormat;

    PCVertex(vec3 iPosition, vec4 iColor);
    /*
    virtual const BufferFormat &getBufferFormat() const { 
        return bufferFormat;
    }
    */
    /*
    virtual void expand(AABB &ioBoundingBox) const {
        ioBoundingBox.expand(position);
    }
    */
};

#endif
