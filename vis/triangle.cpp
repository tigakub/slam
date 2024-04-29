#include "triangle.h"

Triangle::Triangle()
: Mesh() { }

void Triangle::initGeometry() {
    vector<PCVertex> & vertices = vbo.getVertices();

    vertices.emplace_back(PCVertex(vec3(  0.0f, -0.5f,  0.0f ), vec4( 1.0, 0.0, 0.0, 1.0 )));
    vertices.emplace_back(PCVertex(vec3(  1.0f, -0.5f,  0.0f ), vec4( 0.0, 1.0, 0.0, 1.0 )));
    vertices.emplace_back(PCVertex(vec3(  0.5f,  0.5f,  0.0f ), vec4( 0.0, 0.0, 1.0, 1.0 )));

    auto offsetOfPositionX = offsetof(PCVertex, position.x);
    auto offsetOfPosition = offsetof(PCVertex, position);
    auto offsetOfColorX = offsetof(PCVertex, color.x);
    auto offsetOfColor = offsetof(PCVertex, color);
    // auto offsetOfBufFmt = offsetof(PCVertex, bufferFormat);

    PCVertex *vertexPtr = &vertices[0];
    float *floatPtr = &vertexPtr->position.x;

    vector<GLuint> & elements = ebo.getElements();
    
    elements.emplace_back(0);
    elements.emplace_back(1);
    elements.emplace_back(2);
}
