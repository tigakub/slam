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

#include "average.h"
#include "exception.h"
#include "lidar.h"
#include "visualizer.h"
#include "vis/pointCloud.h"

using namespace std;

class Slam {
    protected:
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

        PointCloud pointCloud;

    public:
        Slam()
        : terminate(false), 
          visThreadPtr(nullptr), lidarThreadPtr(nullptr),
          visFreq(0.0), visAvgFreq(100),
          imuFreq(0.0),
          lidarFreq(0.0),
          imuHeartBeat(0), lidarHeartBeat(0),
          outputMutex(),
          waitForFirstPointCloud(1),
          occupancyQueue(),
          occupancyQueueMutex(),
          pointCloud()
        { }

        virtual ~Slam() {
            stop();
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
            try {
                waitForFirstPointCloud.acquire();                
                Visualizer visualizer(occupancyQueue, occupancyQueueMutex, string("slam"), pointCloud);
                waitForFirstPointCloud.release();

                auto lastTime = chrono::high_resolution_clock::now();
                while(!shouldTerminate()) {
                    waitForFirstPointCloud.acquire();                
                    visualizer.update();
                    if(!visualizer.loop()) {
                        setShouldTerminate(true);
                    }
                    waitForFirstPointCloud.release();
                    setVisFreq(visualizer.getFrequency());
                }

            } catch (Exception e) {
                setShouldTerminate(true);
                lockOutput();
                cerr << e << endl;
                unlockOutput();
            }
        }

        void lidarProc() {
            try {
                Lidar lidar(occupancyQueue, occupancyQueueMutex, imuHeartBeat, lidarHeartBeat, imuFreq, lidarFreq, pointCount, pointCloud, waitForFirstPointCloud);
                
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

int main(int argc, char **argv) {
    Slam slam;
    slam.start();

    while(!slam.shouldTerminate()) {
        
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
        
    }

    slam.join();

    return 0;
}
