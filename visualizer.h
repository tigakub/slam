#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include <string>

using namespace std;

class Visualizer {
    protected:
        size_t width, height;
        GLFWwindow * window;

        static const char *vertexShaderSource;
        static const char *fragmentShaderSource;
        unsigned int shaderProgram;

        chrono::high_resolution_clock::time_point lastTimeStamp;
        double frequency;

    public:
        Visualizer(const string &iWindowTitle, size_t iWidth = 800, size_t iHeight = 600);
        virtual ~Visualizer();

        virtual int loop();
        virtual void render();

        void framebufferSizeCallback(size_t iWidth, size_t iHeight);
        void processInput();

        double getFrequency() const;
};

#endif
