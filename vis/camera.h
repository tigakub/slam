#ifndef __CAMERA_H__
#define __CAMERA_H__

// #include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "shaderData.h"
#include "aabb.h"

using namespace glm;

class Camera {
    protected:
        bool dirty;
        float fovy;
        int width;
        int height;
        float fov;
        vec3 eye;
        vec3 center;
        vec3 up;
        CameraData data;

        unsigned int ubo;

    public:
        Camera(int iWidth, int iHeight);
        virtual ~Camera();

        bool init(const AABB &iBoundingBox);

        void resize(int iWidth, int iHeight);
        void setFocus(const AABB &iBoundingBox);

        void update();
};

#endif