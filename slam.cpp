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

#include "average.h"
#include "exception.h"
#include "lidar.h"
#include "visualizer.h"

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

        mutex outputMutex;

        deque<OccupancyGrid *> occupancyQueue;
        mutex occupancyQueueMutex;

    public:
        Slam()
        : terminate(false), 
          visThreadPtr(nullptr), lidarThreadPtr(nullptr),
          visFreq(0.0), visAvgFreq(100),
          imuFreq(0.0),
          lidarFreq(0.0),
          imuHeartBeat(0), lidarHeartBeat(0),
          outputMutex(),
          occupancyQueue(),
          occupancyQueueMutex()
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
            if(!visThreadPtr)
                visThreadPtr = new thread(&Slam::visProc, this);
            if(!lidarThreadPtr)
                lidarThreadPtr = new thread(&Slam::lidarProc, this);
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

        void lockOutput() {
            outputMutex.lock();
        }

        void unlockOutput() {
            outputMutex.unlock();
        }
 
    protected:
        void visProc() {
            try {
                Visualizer visualizer(occupancyQueue, occupancyQueueMutex, string("slam"));

                auto lastTime = chrono::high_resolution_clock::now();
                while(!shouldTerminate()) {
                    visualizer.update();
                    if(!visualizer.loop()) {
                        setShouldTerminate(true);
                    }
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
                Lidar lidar(occupancyQueue, occupancyQueueMutex, imuHeartBeat, lidarHeartBeat, imuFreq, lidarFreq);
                
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

        slam.lockOutput();
        cout 
            << setw(6) << setprecision(0) << fixed 
            << " visfreq: " << visFreq 
             << setprecision(2) << " imuFreq: " << imuFreq
            << " imuHB: " << imuHeartBeat 
            << " lidarFreq: " << lidarFreq
             << setprecision(2) << " lidarHB: " << lidarHeartBeat 
            << "           \r";
        cout.flush();
        slam.unlockOutput();
    }

    slam.join();

    return 0;
}
