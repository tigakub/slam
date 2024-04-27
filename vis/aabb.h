#ifndef __AABB_H__
#define __AABB_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

struct AABB {
    vec3 min;
    vec3 max;

    AABB();

    void expand(const vec3 & iPoint);

    bool valid() const;

    const vec3 & operator[](unsigned int i) const;
};

#endif