#ifndef __LIDAR_H__
#define __LIDAR_H__

#include <string>
#include <deque>
#include <atomic>
#include <mutex>

#include "average.h"
#include "unitree_lidar_sdk.h"
#include "occupancyGrid.h"

using namespace std;
using namespace unitree_lidar_sdk;

class Lidar {
    protected:
        UnitreeLidarReader * reader;
        uint32_t timeDelay;

        deque<OccupancyGrid *> & occupancyQueue;
        mutex & occupancyQueueMutex;

        atomic<uint64_t> & imuHeartBeat;
        atomic<uint64_t> & lidarHeartBeat;

        chrono::high_resolution_clock::time_point lastImuTimeStamp;
        atomic<double> & imuFreq;
        Average imuAvgFreq;

        chrono::high_resolution_clock::time_point lastLidarTimeStamp;
        atomic<double> & lidarFreq;
        Average lidarAvgFreq;

    public:
        Lidar(  
            deque<OccupancyGrid *> & ioOccupancyQueue, 
            mutex & ioOccupancyQueueMutex, 
            atomic<uint64_t> & ioImuHeartBeat, 
            atomic<uint64_t> & ioLidarHeartBeat, 
            atomic<double> & ioImuFreq, 
            atomic<double> & ioLidarFreq, 
            const string & iPortName = string("/dev/serial/by-id/usb-Silicon_Labs_CP2104_USB_to_UART_Bridge_Controller_02AF54A2-if00-port0"));
        virtual ~Lidar();

        void setMode(LidarWorkingMode);

        string getFirmwareVersion() const;
        string getSDKVersion() const;

        bool needsCleaning() const;

        void ledsOff();
        void setLEDMode(LEDDisplayMode);

        void loop();

        void processIMU(const IMUUnitree & iImu) const;
        void processPointCloud(const PointCloudUnitree & iCloud) const;
};

#endif
