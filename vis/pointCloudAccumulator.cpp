#include "vis/pointCloudAccumulator.h"

PointCloudAccumulator::PointCloudAccumulator(size_t iMaxClouds)
: MeshBase(), clouds(), maxClouds(iMaxClouds), nextCloud(0) { }

PointCloudAccumulator::~PointCloudAccumulator() { }

void PointCloudAccumulator::cleanUp() {
    while(clouds.size()) {
        auto next = clouds.back();
        delete next;
        clouds.pop_back();
    }
}

size_t PointCloudAccumulator::getCloudCount() {
    return clouds.size();
}

size_t PointCloudAccumulator::getMaxCount() {
    return maxClouds;
}

PointCloud * PointCloudAccumulator::getCurrentCloud() {
    if(!getCloudCount()) return nullptr;
    return clouds[nextCloud];
}

PointCloud * PointCloudAccumulator::getNextCloud() {
    size_t cloudCount = getCloudCount();
    if(!cloudCount) return nullptr;
    PointCloud * next = clouds[nextCloud];
    nextCloud = (nextCloud + 1) % cloudCount;
    return next;
}

void PointCloudAccumulator::addCloud(PointCloud * iPointCloud) {
    if(getCloudCount() >= maxClouds) return;
    nextCloud = clouds.size();
    clouds.push_back(iPointCloud);
}

ElementBufferBase & PointCloudAccumulator::getEBO() {
    return getCurrentCloud()->getEBO();
}

VertexBufferBase & PointCloudAccumulator::getVBO() {
    return getCurrentCloud()->getVBO();
}

/*
void PointCloudAccumulator::init() {
}
*/

void PointCloudAccumulator::update() {
    getCurrentCloud()->update();
}

void PointCloudAccumulator::draw() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    size_t cloudCount = getCloudCount();
    size_t currentCloud = nextCloud;
    size_t i = cloudCount;
    while(i--) {
        clouds[(currentCloud + i) % cloudCount]->draw();
    }
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void PointCloudAccumulator::expand(AABB & ioBoundingBox) {

}
