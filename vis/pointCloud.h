#ifndef __POINTCLOUD_H__
#define __POINTCLOUD_H__

#include <unitree_lidar_sdk.h>
#include <mutex>
#include <chrono>

using namespace unitree_lidar_sdk;

#include "mesh.h"

class PointCloud: public Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS, true, 2000 * sizeof(GLuint), true, 2000 * sizeof(PCVertex)> {
    protected:
        chrono::time_point<chrono::high_resolution_clock> timeStamp;

    public:
        PointCloud();
        virtual ~PointCloud();

        int64_t age();

        void lockPoints();
        void setPoints(const vector<PointUnitree> &iUnitreePoints);
        void unlockPoints();

        // virtual void init();
        virtual void update();
        virtual void draw();
        virtual void expand(AABB &ioBoundingBox);
        
    protected:
        mutex mtx;
        virtual void initGeometry();
};

#endif
