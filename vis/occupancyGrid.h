#ifndef __OCCUPANCYGRID_H__
#define __OCCUPANCYGRID_H__

#include <stdio.h>
#include <utility>
#include <cstdint> // for uint64_t
#include <chrono>

#include "container.h"
#include "link.h"

using namespace std;

class OccupancyGrid: public Container {
    protected:

        class Cell: public Link {
            protected:
                chrono::time_point<chrono::high_resolution_clock> timestamp;

            public:
                Cell(size_t iIndex);

                void setTimestamp(chrono::time_point<chrono::high_resolution_clock> iTimestamp);
                chrono::time_point<chrono::high_resolution_clock> getTimestamp();

                float age() const;
        };

        class Row: public Container, public Link {
            public:
                Row(size_t iIndex);

                void prune(float iMaxAge = 2.0f);

            protected:
                virtual Link * create(size_t iIndex);
        };

        class Layer: public Container, public Link {
            public:
                Layer(size_t iIndex);

                void prune(float iMaxAge = 2.0f);

            protected:
                virtual Link * create(size_t iIndex);
        };

        size_t gridWidth, gridHeight, gridDepth;
        float minx, maxx;
        float miny, maxy;
        float minz, maxz;
        size_t count;

    public:
        class Functor {
            public:
                virtual void operator()(size_t x, size_t y, size_t z) = 0;
        };

        OccupancyGrid();
        OccupancyGrid(size_t iGridWidth, size_t iGridHeight, size_t iGridDepth, float iMinX, float iMaxX, float iMinY, float iMaxz, float iMinZ, float iMaxZ);
        OccupancyGrid(OccupancyGrid && iOther);
        virtual ~OccupancyGrid();

        void insertCell(float x, float y, float z);

        virtual Link * create(size_t iIndex);

        size_t getCount();

        void map(Functor &functor);

        void prune(float iMaxAge = 2.0f);
};

#endif