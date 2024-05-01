#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

struct Quaternion {
    vec4 value;

    Quaternion();
    Quaternion(const vec4 & iVec);
    // Quaternion(const Quaternion & iOther);
    
    vec4 operator*(const vec4 & iVec) const;

    Quaternion & operator*=(const Quaternion & iOther);
    Quaternion operator*(const Quaternion & iOther) const;
    Quaternion operator~() const;
};

#endif
