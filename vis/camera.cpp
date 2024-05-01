#include "camera.h"
#include "usedsa.h"

Camera::Camera(int iWidth, int iHeight, bool iIsDynamic)
: UniformBuffer(iIsDynamic),
  width(iWidth),
  height(iHeight),
  data() { }

Camera::~Camera() {
}

void Camera::cleanUp() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    if (ubo) glDeleteBuffers(1, &ubo);
}

void Camera::resize(int iWidth, int iHeight) {
    width = iWidth;
    height = iHeight;
    dirty = true;
}

void Camera::setFocus(const AABB &iBoundingBox) {
    AABB box(iBoundingBox);

    if(!box.valid()) {
        box.min = vec3(-1.0f);
        box.max = vec3(1.0f);
    }

    center = (box.min + box.max) * 0.5f;

    up = vec3(0.0f, 1.0f, 0.0f);

    float radius = length(box.max - box.min) * 0.5f;
    float finalDistance = 1.1 * radius / tan(fov * 0.5f);

    vec3 dir(0.0f, 0.0f, 1.0f);

    eye = center + dir * finalDistance;

    dirty = true;
}
void Camera::initData() {
    fov = radians(60.0f);
    eye = vec3(0.0f, -1.0f, 1.0f);
    center = vec3(0.0f, 0.0f, 0.0f);
    up = vec3(0.0f, 0.0f, 1.0f);
    dirty = true;
}

void Camera::update() {
    if(dirty) {
        data.mvMatrix = lookAt(eye, center, up);
        data.projMatrix = perspective(fov, ((float) width / height), 0.1f, 1000.0f);
        
        UniformBuffer::update();
    }
}

const void *Camera::getData() const {
    return (void *) &data;
}

GLuint Camera::getDataSize() const {
    return (GLuint) sizeof(data);
}