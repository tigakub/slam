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

using namespace glm;

class Camera: public UniformBuffer {
    protected:
        int width;
        int height;
        float fov;
        vec3 eye;
        vec3 center;
        vec3 up;
        CameraData data;

    public:
        Camera(int iWidth, int iHeight, GLuint iBindPoint = 0, bool iIsDynamic = false);
        virtual ~Camera();

        void cleanUp();
        
        void setFocus(const AABB &iBoundingBox);

        void resize(int iWidth, int iHeight);

        virtual void update();

    protected:
        virtual const void *getData() const;
        virtual GLuint getDataSize() const;
};

#endif