#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <string>
#include <deque>
#include <atomic>
#include <mutex>
#include <memory>

#include "vis/occupancyGrid.h"

#include "vis/framebuffer.h"
#include "vis/camera.h"
#include "vis/light.h"
#include "vis/aabb.h"
#include "vis/triangle.h"
#include "vis/box.h"
#include "vis/mesh.h"
#include "vis/pointCloudAccumulator.h"
#include "vis/quaternion.h"
#include "vis/node.h"
#include "vis/instanceCloud.h"

using namespace std;
using namespace glm;

class Visualizer: OccupancyGrid::Functor {
    protected:
        bool glAvailable;
        size_t width, height;

        static const char *pointVertexShaderSource;
        static const char *pointFragmentShaderSource;
        static const char *litVertexShaderSource;
        static const char *litInstanceShaderSource;
        static const char *litFragmentShaderSource;
        unsigned int pointShaderProgram;
        unsigned int litShaderProgram;
        unsigned int litInstanceShaderProgram;

        chrono::high_resolution_clock::time_point lastTimeStamp;
        double frequency;

        /*
        deque<OccupancyGrid *> & occupancyQueue;
        mutex & occupancyQueueMutex;
        */
        OccupancyGrid & occupancyGrid;
        mutex & occupancyGridMutex;

        Framebuffer framebuffer;
        Camera camera;
        Light light0;
        Light light1;
        AABB boundingBox;

        PointCloudAccumulator & pcAccum;

        Box gridBox;
        InstanceCloud<InstanceData, Box, false, 0> * grid;
        
        Context context;
        Node rootNode;
        // Box cell;
        // UnmanagedGeometry<Box> *cellGeom;

        // Box testBox;
        // Triangle testTriangle;
        // GLuint testTriangleVAO;

    public:
        Visualizer(
            /*
            deque<OccupancyGrid *> & ioOccupancyQueue, 
            mutex & ioOccupancyQueueMutex, 
            */
            OccupancyGrid & ioOccupancyGrid,
            mutex & ioOccupancyGridMutex,
            PointCloudAccumulator & ioPCAccum, 
            size_t iWidth = 800, 
            size_t iHeight = 600);
        virtual ~Visualizer();
        
        virtual void setViewportSize(GLsizei iWidth, GLsizei iHeight);
        virtual int loop();
        virtual void update();
        virtual void render();

        double getFrequency() const;

        virtual void operator()(size_t x, size_t y, size_t z);

        void setImuQuat(const vec4 & iImuQuat);

    protected:
        string processGLSLSource(const char *iSource);

        bool isDSACompatible();
        bool isDSAExtensionAvailable();
};

#endif
