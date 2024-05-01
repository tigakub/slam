#include "quaternion.h"

Quaternion::Quaternion()
: value(vec4(0.0, 0.0, 0.0, 1.0)) {
}
Quaternion::Quaternion(const vec4 &iVec)
: value(iVec) { }

/*
Quaternion::Quaternion(const Quaternion & iOther)
: value(iOther.value) { }
*/

Quaternion & Quaternion::operator*=(const Quaternion & iOther) {
    return *this;
}

vec4 Quaternion::operator*(const vec4 & iVec) const {
    Quaternion q(*this * Quaternion(iVec) * ~(*this));
    return q.value;
}

Quaternion Quaternion::operator*(const Quaternion & iOther) const {
    return Quaternion(
        vec4(
            value.w * iOther.value.x + value.x * iOther.value.w + value.y * iOther.value.z - value.z * iOther.value.y,
            value.w * iOther.value.y - value.x * iOther.value.z + value.y * iOther.value.w + value.z * iOther.value.x,
            value.w * iOther.value.z + value.x * iOther.value.y - value.y * iOther.value.x + value.z * iOther.value.w,
            value.w * iOther.value.w - value.x * iOther.value.x - value.y * iOther.value.y - value.z * iOther.value.z
        )
    );
}

Quaternion Quaternion::operator~() const {
    return Quaternion(vec4(-value.x, -value.y, -value.z, value.w));
}
