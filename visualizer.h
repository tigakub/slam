#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <string>
#include <deque>
#include <mutex>

#include <occupancyGrid.h>

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

    public:
        Visualizer(deque<OccupancyGrid *> & iOccupancyQueue, mutex & iOccupancyQueueMutex, const string &iWindowTitle, size_t iWidth = 800, size_t iHeight = 600);
        virtual ~Visualizer();

        virtual int loop();
        virtual void update();
        virtual void render();

        void framebufferSizeCallback(size_t iWidth, size_t iHeight);
        void processInput();

        double getFrequency() const;

        virtual void operator()(size_t x, size_t y, size_t z);

    protected:
        string processGLSLSource(const char *iSource);
};

#endif
