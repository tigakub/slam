#ifndef __CAMERA_H__
#define __CAMERA_H__

// #include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "uniformBuffer.h"
#include "shaderData.h"
#include "aabb.h"
#include "vis/quaternion.h"

using namespace glm;

class Camera: public UniformBuffer {
    protected:
        GLsizei width;
        GLsizei height;
        float fov;
        vec3 eye;
        vec3 center;
        vec3 up;
        vec4 imuQuat;
        CameraData data;

    public:
        Camera(GLsizei iWidth, GLsizei iHeight, bool iIsDynamic = true);
        virtual ~Camera();

        CameraData & getCameraData();
        
        void setFocus(const AABB &iBoundingBox);
        void setImuQuat(const vec4 & iImuQuat);

        void resize(GLsizei iWidth, GLsizei iHeight);

        virtual void update();

    protected:
        virtual void initData();
        virtual const void *getData() const;
        virtual GLsizei getDataSize() const;
};

#endif