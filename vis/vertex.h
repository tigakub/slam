#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

    Vertex(vec3 iPosition, vec3 iNormal, vec3 iColor, vec2 iUV)
    : position(iPosition), normal(iNormal), color(iColor), uv(iUV) { }
};

#endif
