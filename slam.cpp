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
#include <mutex>

#include "exception.h"
#include "lidar.h"
#include "visualizer.h"

using namespace std;

class Average {
    protected:
        int periods;
        double *buffer;
        int index;

    public:
        Average(int iPeriods): periods(iPeriods), buffer(new double[periods]), index(0) {
            for(int i = 0; i < periods; i++) {
                buffer[i] = 0.0;
            }
        }
        virtual ~Average() {
            if(buffer) delete [] buffer;
        }

        double operator()(double iCurrent) {
            buffer[index] = iCurrent;
            double sum = 0.0;
            for(int i = 0; i < periods; i++) {
                sum += buffer[i];
            }
            index = (index + 1) % periods;
            return sum / double(periods);
        }
};

class Slam {
    protected:
        bool terminate;
        mutex terminateMutex;

        thread * visThreadPtr;
        thread * lidarThreadPtr;

        double visFreq;
        Average visAvgFreq;
        mutex visFreqMutex;
        double lidarFreq;
        Average lidarAvgFreq;
        mutex lidarFreqMutex;

        mutex outputMutex;

        deque<OccupancyGrid *> occupancyQueue;
        mutex occupancyQueueMutex;

    public:
        Slam()
        : terminate(false), terminateMutex(), 
          visThreadPtr(nullptr), lidarThreadPtr(nullptr),
          visFreq(0.0), visAvgFreq(100), visFreqMutex(),
          lidarFreq(0.0), lidarAvgFreq(100), lidarFreqMutex(),
          outputMutex()
        { }

        virtual ~Slam() {
            stop();
        }

        bool shouldTerminate() {
            bool should = false;
            terminateMutex.lock();
            should = terminate;
            terminateMutex.unlock();
            return should;
        }

        void setShouldTerminate(bool iShould) {
            terminateMutex.lock();
            terminate = iShould;
            terminateMutex.unlock();
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
            double freq;
            visFreqMutex.lock();
            freq = visFreq;
            visFreqMutex.unlock();
            return freq;
        }

        void setVisFreq(double iFreq) {
            visFreqMutex.lock();
            visFreq = iFreq;
            visFreqMutex.unlock();
        }

        double getLidarFreq() {
            double freq;
            lidarFreqMutex.lock();
            freq = lidarFreq;
            lidarFreqMutex.unlock();
            return freq;
        }

        void setLidarFreq(double iFreq) {
            lidarFreqMutex.lock();
            lidarFreq = iFreq;
            lidarFreqMutex.unlock();
        }

        void lockOutput() {
            outputMutex.lock();
        }

        void unlockOutput() {
            outputMutex.unlock();
        }
 
    protected:
        void visProc() {
            Visualizer visualizer(occupancyQueue, occupancyQueueMutex, string("slam"));

            auto lastTime = chrono::high_resolution_clock::now();
            while(!terminate) {
                visualizer.update();
                if(!visualizer.loop()) {
                    setShouldTerminate(true);
                }
                setVisFreq(visualizer.getFrequency());
            }
        }

        void lidarProc() {
            try {
                Lidar lidar(occupancyQueue, occupancyQueueMutex);
                
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

                auto lastTime = chrono::high_resolution_clock::now();
                while(!terminate) {
                    auto start = chrono::high_resolution_clock::now();
                    lidar.loop();
                    auto end = chrono::high_resolution_clock::now();
                    usleep(8250 - chrono::duration_cast<chrono::microseconds>(end - start).count());

                    auto currentTime = chrono::high_resolution_clock::now();
                    auto us = chrono::duration_cast<chrono::microseconds>(currentTime - lastTime).count();
                    setLidarFreq(lidarAvgFreq(1000000.0 / double(us)));
                    lastTime = currentTime;
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
        auto lidarFreq = slam.getLidarFreq();
        slam.lockOutput();
        cout << setw(6) << setprecision(0) << fixed << "vis freq: " << visFreq << "    lidar freq: " << lidarFreq << "\r";
        cout.flush();
        slam.unlockOutput();
    }

    slam.join();

    return 0;
}
