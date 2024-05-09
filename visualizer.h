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

using namespace std;

class Visualizer: OccupancyGrid::Functor {
    protected:
        bool glAvailable;
        size_t width, height;

        static const char *pointVertexShaderSource;
        static const char *pointFragmentShaderSource;
        static const char *vertexLitShaderSource;
        static const char *fragmentLitShaderSource;
        unsigned int pointShaderProgram;
        unsigned int litShaderProgram;

        chrono::high_resolution_clock::time_point lastTimeStamp;
        double frequency;

        deque<OccupancyGrid *> & occupancyQueue;
        mutex & occupancyQueueMutex;

        Framebuffer framebuffer;
        Camera camera;
        Light light;
        AABB boundingBox;

        PointCloudAccumulator & pcAccum;
        
        Context context;
        Node rootNode;

        // Box testBox;
        // Triangle testTriangle;
        // GLuint testTriangleVAO;

    public:
        Visualizer(
            deque<OccupancyGrid *> & ioOccupancyQueue, 
            mutex & ioOccupancyQueueMutex, 
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
