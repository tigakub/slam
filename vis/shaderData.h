#ifdef __cplusplus
#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;
#endif

#ifdef __cplusplus
struct alignas(16) Plane {
#else
struct Plane {
#endif
    float nx, ny, nz;
    float offset;
    int px, py, pz;
};

#ifdef __cplusplus
struct alignas(16) GridData {
#else
struct GridData {
#endif
    float ox, oy, oz;
    float dx, dy, dz;
    int sx, sy, sz;
};

#ifdef __cplusplus
struct alignas(16) InstanceData {
#else
struct InstanceData {
#endif
    float ox, oy, oz;
    float sx, sy, sz;
};

#ifdef __cplusplus
struct alignas(16) FrustumData {
#else
struct FrustumData {
#endif
    Plane near;
    Plane left;
    Plane right;
    Plane bottom;
    Plane top;
    Plane far;
};

#ifdef __cplusplus
struct alignas(16) CameraData {
#else
struct CameraData {
#endif
    mat4 viewMatrix;
    mat4 projMatrix;
    vec4 imuQuat;
    #ifdef __cplusplus
    CameraData() : viewMatrix(1.0f), projMatrix(1.0f), imuQuat(vec4(0.0, 0.0, 0.0, 1.0)) { }
    #endif
};

#ifdef __cplusplus
struct alignas(16) ContextData {
#else
struct ContextData {
#endif
    mat4 modelMatrix;
    vec4 tint;
    #ifdef __cplusplus
    ContextData(): modelMatrix(1.0f), tint(1.0, 1.0, 1.0, 1.0) { }
    #endif
};

#ifdef __cplusplus
struct alignas(16) LightData {
#else
struct LightData {
#endif
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    #ifdef __cplusplus
    LightData(): position(0.0, 0.0, 10.0, 0.0), ambient(0.2, 0.2, 0.2, 1.0), diffuse(1.0, 1.0, 1.0, 1.0), specular(0.0, 0.0, 0.0, 1.0) { }
    #endif
};

#define UB_CAMERA		0
#define UB_MATERIAL		1
#define UB_LIGHT0		2
#define UB_LIGHT1		3
#define UB_LIGHT2		4
#define UB_LIGHT3		5
#define UB_LIGHT4		6
#define UB_LIGHT5		7
#define UB_LIGHT6		8
#define UB_LIGHT7		9

#define IN_POSITION		0
#define IN_NORMAL		1
#define IN_COLOR		2
#define IN_TEXCOORD		3

#define SB_TRANSFORM	0
#define SB_MATERIAL		1
#define SB_TEXTURE		2
#define SB_IN_DRAW_CMD	3
#define SB_OUT_DRAW_CMD	4
#define SB_BOUNDS		5
#define SB_FRUSTUM      6

#define OUT_COLOR       0
#define NUM_FRAG_OUT    1

#ifndef __cplusplus
vec4 hamiltonion(vec4 a, vec4 b) {
    vec4 result;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return result;
}

vec4 invertQuaternion(vec4 q) {
    vec4 result;
    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    result.w = q.w;
    return result;
}

vec4 conjugate(vec4 p, vec4 q) {
    return hamiltonion(hamiltonion(q, p), invertQuaternion(q));
}

mat4 quatToMat(vec4 q) {
    float qx = q.x;
    float qy = q.y;
    float qz = q.z;
    float qw = q.w;

    float xx = qx * qx;
    float yy = qy * qy;
    float zz = qz * qz;
    float xy = qx * qy;
    float xz = qx * qz;
    float yz = qy * qz;
    float wx = qw * qx;
    float wy = qw * qy;
    float wz = qw * qz;

    mat4 result;
    result[0][0] = 1.0 - 2.0 * (yy + zz);
    result[0][1] = 2.0 * (xy + wz);
    result[0][2] = 2.0 * (xz - wy);
    result[0][3] = 0.0;

    result[1][0] = 2.0 * (xy - wz);
    result[1][1] = 1.0 - 2.0 * (xx + zz);
    result[1][2] = 2.0 * (yz + wx);
    result[1][3] = 0.0;

    result[2][0] = 2.0 * (xz + wy);
    result[2][1] = 2.0 * (yz - wx);
    result[2][2] = 1.0 - 2.0 * (xx + yy);
    result[2][3] = 0.0;

    result[3][0] = 0.0;
    result[3][1] = 0.0;
    result[3][2] = 0.0;
    result[3][3] = 1.0;

    return result;
}

#endif
