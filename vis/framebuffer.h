#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

//#include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "usedsa.h"

using namespace glm;

class Framebuffer {
    protected:
        GLsizei width;
        GLsizei height;
        GLuint fbo;
        GLuint colorTex;
        GLuint depthTex;

    public:
        Framebuffer(int iWidth, int iHeight);
        virtual ~Framebuffer();
        
        bool init();

        int getWidth() const { return width; }
        int getHeight() const { return height; }

        void resize(int iWidth, int iHeight);
        void clear();
        void bind();
        void unbind();
        void blt();
};

#endif
