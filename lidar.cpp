#include "lidar.h"
#include "exception.h"

Lidar::Lidar(deque<OccupancyGrid *> & iOccupancyQueue, mutex & iOccupancyQueueMutex, const string &iPortName)
: occupancyQueue(iOccupancyQueue), occupancyQueueMutex(iOccupancyQueueMutex) {
    reader = createUnitreeLidarReader();    
    int cloudScanNum = 18;
    
    THROW_IF(
        reader->initialize(cloudScanNum, iPortName),
        "Unable to create UnitreeLidarRadar");

    timeDelay = reader->getTimeDelay() * 2;
}

Lidar::~Lidar() {
}

void Lidar::setMode(LidarWorkingMode iMode) {
    reader->setLidarWorkingMode(iMode);
    usleep(timeDelay);
}

string Lidar::getFirmwareVersion() const {
    string result("unknown");
    while(true) {
        if(reader->runParse() == VERSION) {
            result = reader->getVersionOfFirmware();
            break;
        }
    }
    usleep(timeDelay);
    return result;
}

string Lidar::getSDKVersion() const {
    return reader->getVersionOfSDK();
}

bool Lidar::needsCleaning() const {
    int countPercentage = 0;
    while(true) {
        if(++countPercentage > 2) {
            break;
        }
        if(reader->getDirtyPercentage() > 10) {
            return true;
        }
        usleep(timeDelay);
    }
    return false;
}

void Lidar::ledsOff() {
    int i = 45;
    uint8_t ledTable[45];
    while(--i) {
        ledTable[i] = 0;
    }
    reader->setLEDDisplayMode(ledTable);
    usleep(timeDelay);
}

void Lidar::setLEDMode(LEDDisplayMode iMode) {
    reader->setLEDDisplayMode(iMode);
    usleep(timeDelay);
}

void Lidar::loop() {
    MessageType result = reader->runParse();

    switch (result) {
        case IMU: processIMU(reader->getIMU()); break;
        case POINTCLOUD: processPointCloud(reader->getCloud()); break;
    }
    usleep(timeDelay);
}

void Lidar::processIMU(const IMUUnitree & iImu) const {
    auto stamp = iImu.stamp;
    auto id = iImu.id;
    auto &q0 = iImu.quaternion[0];
    auto &q1 = iImu.quaternion[1];
    auto &q2 = iImu.quaternion[2];
    auto &q3 = iImu.quaternion[3];
    auto timeDelay = reader->getTimeDelay();
}

void Lidar::processPointCloud(const PointCloudUnitree & iCloud) const {
    auto stamp = iCloud.stamp;
    auto id = iCloud.id;
    auto ringNum = iCloud.ringNum;
    auto &points = iCloud.points;
    auto pointCount = points.size();

    float minx = 1000.0, maxx = -1000.0;
    float miny = 1000.0, maxy = -1000.0;
    float minz = 1000.0, maxz = -1000.0;

    for(size_t i = 0; i < pointCount; i++) {
        auto point = points[i];
        if(point.x < minx) minx = point.x;
        if(maxx < point.x) maxx = point.x;
        if(point.y < miny) miny = point.y;
        if(maxy < point.y) maxy = point.y;
        if(point.z < minz) minz = point.z;
        if(maxz < point.z) maxz = point.z;
    }

    OccupancyGrid *grid = new OccupancyGrid(1000, 1000, 1000, minx, maxx, miny, maxy, minz, maxz);

    for(size_t i = 0; i < pointCount; i++) {
        auto point = points[i];
        grid->insertCell(point.x, point.y, point.z);
    }

    occupancyQueueMutex.lock();
    occupancyQueue.push_back(grid);
    occupancyQueueMutex.unlock();
    
    auto timeDelay = reader->getTimeDelay();
}
