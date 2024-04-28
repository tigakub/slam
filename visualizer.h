#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <string>
#include <deque>
#include <atomic>
#include <mutex>

#include <occupancyGrid.h>

#include "vis/framebuffer.h"
#include "vis/camera.h"
#include "vis/light.h"
#include "vis/aabb.h"
#include "vis/box.h"

using namespace std;

class Visualizer: OccupancyGrid::Functor {
    protected:
        bool glAvailable;
        size_t width, height;
        GLFWwindow * window;

        static const char *vertexShaderSource;
        static const char *fragmentShaderSource;
        static const char *vertexShaderSource2;
        static const char *fragmentShaderSource2;
        unsigned int shaderProgram;

        chrono::high_resolution_clock::time_point lastTimeStamp;
        double frequency;

        deque<OccupancyGrid *> & occupancyQueue;
        mutex & occupancyQueueMutex;

        Framebuffer framebuffer;
        Camera camera;
        Light light;
        AABB boundingBox;

        Box testBox;
        GLuint testTriangleVAO;

    public:
        Visualizer(deque<OccupancyGrid *> & ioOccupancyQueue, mutex & ioOccupancyQueueMutex, const string &iWindowTitle, size_t iWidth = 800, size_t iHeight = 600);
        virtual ~Visualizer();

        void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message) const;

        virtual int loop();
        virtual void update();
        virtual void render();

        void framebufferSizeCallback(size_t iWidth, size_t iHeight);
        void processInput();

        double getFrequency() const;

        virtual void operator()(size_t x, size_t y, size_t z);

    protected:
        string processGLSLSource(const char *iSource);

        bool isDSACompatible();
        bool isDSAExtensionAvailable();
};

#endif
