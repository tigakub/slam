#ifdef __cplusplus
#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;
#endif

struct Plane {
    float nx, ny, nz;
    float offset;
    int px, py, pz;
};

struct FrustumData {
    Plane near;
    Plane left;
    Plane right;
    Plane bottom;
    Plane top;
    Plane far;
};

struct CameraData {
    mat4 mvMatrix;
    mat4 projMatrix;
    vec4 imuQuat;
    #ifdef __cplusplus
    CameraData() : mvMatrix(1.0f), projMatrix(1.0f), imuQuat(vec4(0.0, 0.0, 0.0, 1.0)) { }
    #endif
};

struct LightData {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
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
