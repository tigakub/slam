#include <iostream>

using namespace std;

#include "pointCloud.h"

PointCloud::PointCloud()
: Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)>(true, true), mtx() {
    vector<PCVertex> & vData = getVertexData();
    vData.reserve(2000);
    vector<GLuint> & eData = getElementData();
    eData.reserve(2000);
}

PointCloud::~PointCloud() { }

void PointCloud::lockPoints() {
    mtx.lock();
}

int64_t PointCloud::age() {
    auto elapsed = chrono::high_resolution_clock::now() - timeStamp;
    return static_cast<int64_t>(chrono::duration_cast<chrono::nanoseconds>(elapsed).count());
}

void PointCloud::setPoints(const vector<PointUnitree> &iUnitreePoints) {
    timeStamp = chrono::high_resolution_clock::now();
    lockPoints();
    size_t max =  2000;
    if(max > iUnitreePoints.size()) max = iUnitreePoints.size();
    vector<PCVertex> & vData = vbo.getVertices(); // getVertexData();
    vData.clear();
    for(size_t i = 0; i < max; i++) {
        float x = iUnitreePoints[i].x;
        float y = iUnitreePoints[i].y;
        float z = iUnitreePoints[i].z;
        float r = 1.0;
        float g = 1.0;
        float b = 1.0;
        vData.emplace_back(PCVertex(vec3(x, y, z), vec4(r, g, b, 1.0)));
    }
    vbo.mark();

    vector<GLuint> & eData = getElementData();
    eData.clear();
    for(GLuint i = 0; i < max; i++) {
        eData.push_back(i);
    }
    ebo.mark();
    unlockPoints();
}

void PointCloud::unlockPoints() {
    mtx.unlock();
}

void PointCloud::initGeometry() { }

/*
void PointCloud::init() {
    lockPoints();
    Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)>::init();
    unlockPoints();
}
*/

void PointCloud::update() {
    lockPoints();
    Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)>::update();
    unlockPoints();
}

void PointCloud::draw() {
    lockPoints();
    Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)>::draw();
    unlockPoints();
}

void PointCloud::expand(AABB & ioBoundingBox) {
    lockPoints();
    Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)>::expand(ioBoundingBox);
    unlockPoints();
}
