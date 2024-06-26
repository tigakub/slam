#ifndef __POINTCLOUDACCUMULATOR_H__
#define __POINTCLOUDACCUMULATOR_H__

#include <vector>
#include <memory> // for shared_ptr
#include <mutex>

using namespace std;

#include "pointCloud.h"

class PointCloudAccumulator : MeshBase {
    protected:
        mutex accessMutex;
        vector<PointCloud *> clouds;
        size_t maxClouds;
        size_t nextCloud;

    public:
        PointCloudAccumulator(size_t iMaxClouds = 8);
        virtual ~PointCloudAccumulator();

        void cleanUp();
        
        size_t getCloudCount();
        size_t getMaxCount();
        PointCloud * getCurrentCloud();
        PointCloud * getNextCloud();
        void addCloud(PointCloud * iCloud);

        virtual ElementBufferBase & getEBO();
        virtual VertexBufferBase & getVBO();

        // virtual void init();
        virtual void update();
        virtual void draw();
        virtual void expand(AABB & ioBoundingBox);
};

#endif
