#include "aabb.h"

using namespace std;

AABB::AABB()
: min(vec3(numeric_limits<float>::max())),
  max(vec3(-numeric_limits<float>::max())) { }

void AABB::expand(const vec3 & iPoint) {
    min = glm::min(min, iPoint);
    max = glm::max(max, iPoint);
}

bool AABB::valid() const {
    return min.x <= max.x && min.y <= max.y && min.z <= max.z;
}

const vec3 & AABB::operator[](unsigned int i) const {
    return *(&min + i);
}
