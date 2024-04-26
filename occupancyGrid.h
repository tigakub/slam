#ifndef __OCCUPANCYGRID_H__
#define __OCCUPANCYGRID_H__

#include <stdio.h>

#include "container.h"
#include "link.h"

using namespace std;

class OccupancyGrid: public Container {
    protected:

        class Cell: public Link {
            public:
                Cell(size_t iIndex);
        };

        class Row: public Container, public Link {
            public:
                Row(size_t iIndex);

            protected:
                virtual Link * create(size_t iIndex);
        };

        class Layer: public Container, public Link {
            public:
                Layer(size_t iIndex);

            protected:
                virtual Link * create(size_t iIndex);
        };

        size_t gridWidth, gridHeight, gridDepth;
        float minx, maxx;
        float miny, maxy;
        float minz, maxz;

    public:
        OccupancyGrid(size_t iGridWidth, size_t iGridHeight, size_t iGridDepth, float iMinX, float iMaxX, float iMinY, float iMaxz, float iMinZ, float iMaxZ);
        virtual ~OccupancyGrid();

        void insertCell(float x, float y, float z);

        virtual Link * create(size_t iIndex);
};

#endif