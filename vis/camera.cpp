#include "camera.h"

Camera::Camera(GLuint iBindPoint, GLsizei iWidth, GLsizei iHeight, bool iIsDynamic)
: UniformBuffer(iBindPoint, iIsDynamic),
  width(iWidth),
  height(iHeight),
  data() {  }

void Camera::resize(GLsizei iWidth, GLsizei iHeight) {
    width = iWidth;
    height = iHeight;
    dirty = true;
}

CameraData & Camera::getCameraData() {
    return data;
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

void Camera::setImuQuat(const vec4 & iImuQuat) {
    imuQuat = iImuQuat;

    dirty = true;
}

void Camera::initData() {
    fov = radians(90.0f);
    eye = vec3(0.5f, 0.5f, 0.5f);
    center = vec3(0.0f, 0.0f, 0.25f);
    up = vec3(0.0f, 0.0f, 1.0f);
    dirty = true;
}

void Camera::update() {
    if(dirty) {
        data.viewMatrix = lookAt(eye, center, up);
        data.projMatrix = perspective(fov, ((float) width / height), 0.1f, 1000.0f);
        data.imuQuat = imuQuat;
        UniformBuffer::update();
    }
}

const void *Camera::getData() const {
    return (void *) & data;
}

GLsizei Camera::getDataSize() const {
    return (GLsizei) sizeof(data);
}