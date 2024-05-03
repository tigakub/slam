#include "slamConfig.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <semaphore>
#include <memory> // for shared_ptr

#include "average.h"
#include "exception.h"
#include "lidar.h"
#include "visualizer.h"
#include "vis/pointCloud.h"
#include "vis/quaternion.h"

using namespace std;

class Slam;

void gFramebufferSizeCallback(GLFWwindow * iWindow, int iWidth, int iHeight);
void gDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam);

class Slam {
    protected:
        GLFWwindow * window;
        GLsizei width, height;

        atomic<bool> terminate;

        thread * visThreadPtr;
        thread * lidarThreadPtr;

        atomic<double> visFreq;
        Average visAvgFreq;
        
        atomic<double> imuFreq;
        atomic<double> lidarFreq;

        atomic<uint64_t> imuHeartBeat;
        atomic<uint64_t> lidarHeartBeat;

        atomic<uint64_t> pointCount;

        mutex outputMutex;
        binary_semaphore waitForFirstPointCloud;

        deque<OccupancyGrid *> occupancyQueue;
        mutex occupancyQueueMutex;

        // shared_ptr<PointCloud> pointCloud;
        PointCloud * pointCloud;

        vec4 imuQuat;
        mutex imuQuatMutex;

    public:
        Slam()
        : window(nullptr),
          width(800), height(600),
          terminate(false), 
          visThreadPtr(nullptr), lidarThreadPtr(nullptr),
          visFreq(0.0), visAvgFreq(100),
          imuFreq(0.0),
          lidarFreq(0.0),
          imuHeartBeat(0), lidarHeartBeat(0),
          outputMutex(),
          waitForFirstPointCloud(1),
          occupancyQueue(),
          occupancyQueueMutex(),
          pointCloud(new PointCloud) { }

        virtual ~Slam() {
            stop();
        }

        void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message) const {
            cerr << message << endl;
        }

        void framebufferSizeCallback(size_t iWidth, size_t iHeight) {
            width = iWidth;
            height = iHeight;
            glViewport(0, 0, iWidth, iWidth);
        }

        void processInput() {
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
        }

        bool shouldTerminate() {
            return terminate;
        }

        void setShouldTerminate(bool iShould) {
            terminate = iShould;
        }

        void start() {
            if(!lidarThreadPtr) {
                waitForFirstPointCloud.acquire();
                lidarThreadPtr = new thread(&Slam::lidarProc, this);
            }
            if(!visThreadPtr) {
                visThreadPtr = new thread(&Slam::visProc, this);
            }
        }

        void stop() {
            setShouldTerminate(true);
            join();
        }

        void join() {
            if(lidarThreadPtr) {
                lidarThreadPtr->join();
                delete lidarThreadPtr;
                lidarThreadPtr = nullptr;
            }

            if(visThreadPtr) {
                visThreadPtr->join();
                delete visThreadPtr;
                visThreadPtr = nullptr;
            }
        }

        double getVisFreq() {
            return visFreq;
        }

        void setVisFreq(double iFreq) {
            visFreq = iFreq;
        }

        double getImuFreq() {
            return imuFreq;
        }

        double getLidarFreq() {
            return lidarFreq;
        }

        uint64_t getImuHeartBeat() {
            return imuHeartBeat;
        }

        uint64_t getLidarHeartBeat() {
            return lidarHeartBeat;
        }

        size_t getPointCount() {
            return pointCount;
        }

        void lockOutput() {
            outputMutex.lock();
        }

        void unlockOutput() {
            outputMutex.unlock();
        }
 
    protected:
        void visProc() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            window = glfwCreateWindow(width, height, "Unitree 4D LiDAR L1", NULL, NULL);
            THROW_IF_NULL(window, "Failed to create GLFW window");

            glfwMakeContextCurrent(window);
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, gFramebufferSizeCallback);

            THROW_IF_NOT(gladLoadGL(glfwGetProcAddress), "Failed to initialize GLAD");

            glEnable(GL_DEBUG_OUTPUT);

            glDebugMessageCallback(gDebugMessageCallback, static_cast<void *>(this));

            try {
                waitForFirstPointCloud.acquire();                
                Visualizer visualizer(occupancyQueue, occupancyQueueMutex, *pointCloud, width, height);
                waitForFirstPointCloud.release();

                auto lastTime = chrono::high_resolution_clock::now();
                while(!shouldTerminate()) {
                    if(glfwWindowShouldClose(window)) {
                        setShouldTerminate(true);
                    }
                    visualizer.setViewportSize(width, height);
                    waitForFirstPointCloud.acquire();   
                    imuQuatMutex.lock();
                    visualizer.setImuQuat(imuQuat);
                    imuQuatMutex.unlock();
                    visualizer.update();
                    visualizer.loop();
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    waitForFirstPointCloud.release();
                    setVisFreq(visualizer.getFrequency());
                    processInput();
                }

            } catch (Exception e) {
                setShouldTerminate(true);
                lockOutput();
                cerr << e << endl;
                unlockOutput();
            }
            if(pointCloud) delete pointCloud;
            pointCloud = nullptr;
            glfwTerminate();
        }

        void lidarProc() {
            try {
                Lidar lidar(occupancyQueue, occupancyQueueMutex, imuHeartBeat, lidarHeartBeat, imuFreq, lidarFreq, pointCount, *pointCloud, waitForFirstPointCloud);
                
                // lidar.setMode(STANDBY);
                // sleep(1);
                
                lidar.setMode(NORMAL);
                sleep(1);

                auto firmwareVersion = lidar.getFirmwareVersion();
                auto sdkVersion = lidar.getSDKVersion();

                lockOutput();
                cout << "4D LiDAR L1 firmware version " << firmwareVersion << endl;
                cout << "unitree_lidar_sdk version " << sdkVersion << endl;
                unlockOutput();

                while(!terminate) {
                    auto start = chrono::high_resolution_clock::now();
                    imuQuatMutex.lock();
                    imuQuat = lidar.getImuQuat();
                    imuQuatMutex.unlock();
                    lidar.loop();
                    auto end = chrono::high_resolution_clock::now();
                    // usleep(8250 - chrono::duration_cast<chrono::microseconds>(end - start).count());
                }
            } catch (Exception e) {
                lockOutput();
                cerr << e << endl;
                unlockOutput();
            }
        }
};

void gFramebufferSizeCallback(GLFWwindow * iWindow, int iWidth, int iHeight) {
    Slam *ptr = static_cast<Slam *>(glfwGetWindowUserPointer(iWindow));
    ptr->framebufferSizeCallback(iWidth, iHeight);
}

void gDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
    const Slam *ptr = static_cast<const Slam *>(userParam);
    if(ptr) ptr->debugMessageCallback(source, type, id, severity, length, message);

}

int main(int argc, char **argv) {
    Slam slam;
    slam.start();

    while(!slam.shouldTerminate()) {
        /*
        auto visFreq = slam.getVisFreq();
        auto imuHeartBeat = slam.getImuHeartBeat();
        auto imuFreq = slam.getImuFreq();
        auto lidarHeartBeat = slam.getLidarHeartBeat();
        auto lidarFreq = slam.getLidarFreq();
        auto pointCount = slam.getPointCount();

        slam.lockOutput();
        cout 
            << setw(6) << setprecision(0) << fixed 
            << " visfreq: " << visFreq 
            << setprecision(2) << " imuFreq: " << imuFreq
            << " imuHB: " << imuHeartBeat 
            << " lidarFreq: " << lidarFreq
            << setprecision(2) << " lidarHB: " << lidarHeartBeat 
            << " point count: " << pointCount
            << "           \r";
        cout.flush();
        slam.unlockOutput();
        */
    }

    slam.join();

    return 0;
}
