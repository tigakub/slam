#ifndef __BOX_H__
#define __BOX_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

using namespace std;
using namespace glm;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
};

class Box {
    protected:
        vector<Vertex> vertices;
        vector<uint32_t> elements;

    public:
        Box(float iWidth = 1.0f, float iHeight = 1.0f, float iDepth = 1.0f);
};

#endif