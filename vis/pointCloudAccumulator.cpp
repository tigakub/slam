#include "vis/pointCloudAccumulator.h"

PointCloudAccumulator::PointCloudAccumulator(size_t iMaxClouds)
: MeshBase(), accessMutex(), clouds(), maxClouds(iMaxClouds), nextCloud(0) { }

PointCloudAccumulator::~PointCloudAccumulator() { }

void PointCloudAccumulator::cleanUp() {
    accessMutex.lock();
    while(clouds.size()) {
        auto next = clouds.back();
        delete next;
        clouds.pop_back();
    }
    accessMutex.unlock();
}

size_t PointCloudAccumulator::getCloudCount() {
    size_t cloudSize = 0;
    accessMutex.lock();
    cloudSize = clouds.size();
    accessMutex.unlock();
    return cloudSize;
}

size_t PointCloudAccumulator::getMaxCount() {
    return maxClouds;
}

PointCloud * PointCloudAccumulator::getCurrentCloud() {
    PointCloud * pointCloud = nullptr;
    accessMutex.lock();
    if(clouds.size())
        pointCloud = clouds[nextCloud];
    accessMutex.unlock();
    return pointCloud;
}

PointCloud * PointCloudAccumulator::getNextCloud() {
    PointCloud * next = nullptr;
    accessMutex.lock();
    size_t cloudCount = clouds.size();
    if(cloudCount) {
        next = clouds[nextCloud];
        nextCloud = (nextCloud + 1) % cloudCount;
    }
    accessMutex.unlock();
    return next;
}

void PointCloudAccumulator::addCloud(PointCloud * iPointCloud) {
    accessMutex.lock();
    if(clouds.size() < maxClouds) {
        nextCloud = clouds.size();
        clouds.push_back(iPointCloud);
    }
    accessMutex.unlock();
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
    // glDisable(GL_DEPTH_TEST);
    accessMutex.lock();
    glDepthMask(false);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    size_t cloudCount = clouds.size();
    size_t currentCloud = nextCloud;
    size_t i = cloudCount;
    while(i--) {
        clouds[(currentCloud + i) % cloudCount]->draw();
    }
    glDisable(GL_BLEND);
    glDepthMask(true);
    // glEnable(GL_DEPTH_TEST);
    accessMutex.unlock();
}

void PointCloudAccumulator::expand(AABB & ioBoundingBox) {

}
