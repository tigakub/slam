#include "framebuffer.h"
#include "shaderData.h"

Framebuffer::Framebuffer(int iWidth, int iHeight)
: width(iWidth), height(iHeight) {

}

Framebuffer::~Framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if(fbo) glDeleteFramebuffers(1, &fbo);
}

bool Framebuffer::init() {
    #ifdef USEDSA
        glCreateTextures(GL_TEXTURE_2D, 1, &colorTex);
        glTextureStorage2D(colorTex, 1 GL_RGBA, width, height);

        /*
        glTextureImage2DEXT(
            colorTex,
            GL_TEXTURE_2D,
            0,
            GL_RGB8,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr);
        */

        glCreateTextures(GL_TEXTURE_2D, 1, &depthTex);
        glTextureStorage(depthTex, 1, GL_RGBA, width, height);

        /*
        glTextureImage2DEXT(
            depthTex,
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT32F,
            width,
            height,
            0,
            GL_DEPTH_COMPONENT,
            GL_UNSIGNED_BYTE,
            nullptr);
        */

        glCreateFramebuffers(1, &fbo);
        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colorTex, 0);
        glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTex, 0);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0 }
        glNamedFramebufferDrawBuffers(fbo, 1, buffers);

        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        }
   #else
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT32F,
            width,
            height,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #endif
    
    return true;
}

void Framebuffer::resize(int iWidth, int iHeight) {
    width = iWidth;
    height = iHeight;

    #ifdef USEDSA

        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, 0, 0);
        glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, 0, 0);

        glTextureStorage2D(colorTex, 1, GL_RGBA8, width, height);
        glTextureStorage2d(depthTex, 1, GL_DEPTH_COMPONENT32F, width, height);

        glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colorTex, 0);
        glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTex, 0);

        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        }
    #else
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

        glDeleteTextures(1, &colorTex);
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);

        glDeleteTextures(1, &depthTex);
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT32F,
            width,
            height,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            0);

        if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #endif
}
void Framebuffer::clear() {
    #ifdef USEDSA
        float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glClearNamedFramebufferfv(fbo, GL_COLOR, OUT_COLOR, clearColor);

        float clearDepth = 1.0f;
        glClearNamedFramebufferfv(fbo, GL_DEPTH, 0, &clearDepth);
    #else
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | GL_STENCIL_BUFFER_BIT);
        glClearDepth(1.0f);
        glClearStencil(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #endif
}
void Framebuffer::bind() {
    GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
    glNamedFramebufferDrawBuffers(fbo, 2, bufs);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Framebuffer::blt() {
    #ifdef USEDSA
        glNamedFramebufferReadBuffer(fbo, GL_COLOR_ATTACHMENT0);
        glNamedFramebufferDrawBuffer(0, GL_BACK);

        glBlitNamedFramebuffer(fbo, 0, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    #else
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glDrawBuffer(GL_BACK);

        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #endif
}
