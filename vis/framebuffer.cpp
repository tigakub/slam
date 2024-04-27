#include "framebuffer.h"
#include "shaderData.h"

Framebuffer::Framebuffer(int iWidth, int iHeight)
: width(iWidth), height(iHeight) {

}

bool Framebuffer::initialize() {
    glCreateTextures(GL_TEXTURE_2D, 1, &colorTex);
    glTextureImage2DEXT(
        colorTex, 
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        width, 
        height, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        nullptr);

    glCreateTextures(GL_TEXTURE_2D, 1, &depthTex);
    glTextureImage2DEXT(
        depthTex, 
        GL_TEXTURE_2D, 
        0, 
        GL_DEPTH_COMPONENT32F, 
        width, 
        height, 
        0, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        nullptr);

    return true;
}

void Framebuffer::resize(int iWidth, int iHeight) {
    width = iWidth;
    height = iHeight;

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
}
void Framebuffer::clear() {
    float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glClearNamedFramebufferfv(fbo, GL_COLOR, OUT_COLOR, clearColor);

    float clearDepth = 1.0f;
    glClearNamedFramebufferfv(fbo, GL_DEPTH, 0, &clearDepth);
}
void Framebuffer::bind() {
    GLenum bufs[NUM_FRAG_OUT];
    bufs[OUT_COLOR] = GL_COLOR_ATTACHMENT0;
    glNamedFramebufferDrawBuffers(fbo, NUM_FRAG_OUT, bufs);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void Framebuffer::blt() {
    glNamedFramebufferReadBuffer(fbo, GL_COLOR_ATTACHMENT0);
    glNamedFramebufferDrawBuffer(0, GL_BACK);

    glBlitNamedFramebuffer(fbo, 0, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
