#include "camera.h"

Camera::Camera(int iWidth, int iHeight)
: dirty(true),
  width(iWidth),
  height(iHeight),
  fov(radians(60.0f)),
  eye(vec3(0.0f, 10.0f, 0.0f)),
  center(vec3(0.0f, 0.0f, 0.0f)),
  up(vec3(0.0f, 0.0f, 1.0f)),
  ubo(0) { }

Camera::~Camera() {
    if(ubo) glDeleteBuffers(1, &ubo);
}

bool Camera::init(const AABB &iBoundingBox) {
    setFocus(iBoundingBox);

    glCreateBuffers(1, &ubo);
    glNamedBufferStorage(ubo, sizeof(CameraData), &data, GL_DYNAMIC_STORAGE_BIT);

    return true;
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

    up = vec3(1.0f, 1.0f, 1.0f);

    float radius = length(box.max - box.min) * 0.5f;
    float finalDistance = 1.1 * radius / tan(fov * 0.5f);

    vec3 dir(1.0f, 1.0f, 0.0f);

    eye = center + dir * finalDistance;

    dirty = true;
}

void Camera::update() {
    if(dirty) {
        data.mvMatrix = lookAt(eye, center, up);
        data.projMatrix = perspective(fov, ((float) width / height), 0.1f, 1000.0f) * data.mvMatrix;
        glNamedBufferSubData(ubo, 0, sizeof(CameraData), &data);
        dirty = false;
    }
}
