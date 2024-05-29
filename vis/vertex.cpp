#include "vertex.h"

PointScaleVertex::PointScaleVertex(vec3 iPosition, vec3 iScale)
: position(iPosition), scale(iScale) { }

BufferFormat PointScaleVertex::initBufferFormat() {
    BufferFormat fmt;
    fmt.addFormat(3, GL_FLOAT, 3 * sizeof(float));
    fmt.addFormat(3, GL_FLOAT, 3 * sizeof(float));
    return fmt;
}

BufferFormat PointScaleVertex::bufferFormat(PointScaleVertex::initBufferFormat());

PCVertex::PCVertex(vec3 iPosition, vec4 iColor)
: position(iPosition), color(iColor) { }

BufferFormat PCVertex::initBufferFormat() {
    BufferFormat fmt;
    fmt.addFormat(3, GL_FLOAT, 3 * sizeof(float));
    fmt.addFormat(4, GL_FLOAT, 4 * sizeof(float));
    return fmt;
}

BufferFormat PCVertex::bufferFormat(PCVertex::initBufferFormat());

PNCUVertex::PNCUVertex(vec3 iPosition, vec3 iNormal, vec4 iColor, vec2 iUV)
: position(iPosition), normal(iNormal), color(iColor), uv(iUV) { }

BufferFormat PNCUVertex::initBufferFormat() {
    BufferFormat fmt;
    fmt.addFormat(3, GL_FLOAT, 3 * sizeof(float));
    fmt.addFormat(3, GL_FLOAT, 3 * sizeof(float));
    fmt.addFormat(4, GL_FLOAT, 4 * sizeof(float));
    fmt.addFormat(2, GL_FLOAT, 2 * sizeof(float));
    return fmt;
}

BufferFormat PNCUVertex::bufferFormat(PNCUVertex::initBufferFormat());
