#include "vis/occupancyGrid.h"
#include <vector>
#include <cmath>

using namespace std;

OccupancyGrid::Cell::Cell(size_t iIndex)
: Link(iIndex), timestamp() { }

void OccupancyGrid::Cell::setTimestamp(chrono::time_point<chrono::high_resolution_clock> iTimestamp) {
    timestamp = iTimestamp;
}

chrono::time_point<chrono::high_resolution_clock> OccupancyGrid::Cell::getTimestamp() {
    return timestamp;
}

float OccupancyGrid::Cell::age() const {
    const auto start = timestamp;
    const auto now = chrono::high_resolution_clock::now();
    auto elapsedNS = static_cast<uint64_t>(chrono::duration_cast<chrono::nanoseconds>(now - start).count());
    return double(elapsedNS) * 0.000000001;
}

OccupancyGrid::Row::Row(size_t iIndex)
: Container(), Link(iIndex) { }

Link * OccupancyGrid::Row::create(size_t iIndex) {
    return new OccupancyGrid::Cell(iIndex);
}

void OccupancyGrid::Row::prune(float iMaxAge) {
    Cell * cell = static_cast<Cell *>(head);
    while(cell) {
        if(cell->age() >= iMaxAge) {
            removeLink(cell->getIndex());
            cell = nullptr;
        } else {
            cell = static_cast<Cell *>(cell->getNext());
        }
    }
}

OccupancyGrid::Layer::Layer(size_t iIndex)
: Container(), Link(iIndex) { }

Link * OccupancyGrid::Layer::create(size_t iIndex) {
    return new OccupancyGrid::Row(iIndex);
}

void OccupancyGrid::Layer::prune(float iMaxAge) {
    Row * row = static_cast<Row *>(head);
    vector<Row *> rowsToPrune;
    while(row) {
        row->prune(iMaxAge);
        if(!row->getHead()) {
            rowsToPrune.push_back(row);
        }
        row = static_cast<Row *>(row->getNext());
    }

    for(auto row: rowsToPrune) {
        removeLink(row->getIndex());
    }
}

OccupancyGrid::OccupancyGrid()
: Container(), gridWidth(1000), gridHeight(1000), gridDepth(1000), minx(-1.0), maxx(1.0), miny(-1.0), maxy(1.0), minz(-1.0), maxz(1.0) { }

OccupancyGrid::OccupancyGrid(size_t iGridWidth, size_t iGridHeight, size_t iGridDepth, float iMinX, float iMaxX, float iMinY, float iMaxY, float iMinZ, float iMaxZ)
: Container(), gridWidth(iGridWidth), gridHeight(iGridHeight), gridDepth(iGridDepth), minx(iMinX), maxx(iMaxX), miny(iMinY), maxy(iMaxY), minz(iMinZ), maxz(iMaxZ) { }

OccupancyGrid::OccupancyGrid(OccupancyGrid &&iOther)
: Container(::move(iOther)), gridWidth(iOther.gridWidth), gridHeight(iOther.gridHeight), gridDepth(iOther.gridDepth), minx(iOther.minx), maxx(iOther.maxx), miny(iOther.miny), maxy(iOther.maxy), minz(iOther.minz), maxz(iOther.maxz) { }

OccupancyGrid::~OccupancyGrid() { }

Link * OccupancyGrid::create(size_t iIndex) {
    return new OccupancyGrid::Layer(iIndex);
}

void OccupancyGrid::insertCell(float x, float y, float z) {
    if(x < minx) return;
    if(x > maxx) return;
    if(y < miny) return;
    if(y > maxy) return;
    if(z < minz) return;
    if(z > maxz) return;

    auto zFract = (z - minz) / (maxz - minz);
    if(zFract < 0.0) zFract = 0.0;
    if(zFract > 1.0) zFract = 1.0;
    auto zIndex = size_t(floor(gridDepth * zFract));

    auto yFract = (y - miny) / (maxy - miny);
    if(yFract < 0.0) yFract = 0.0;
    if(yFract > 1.0) yFract = 1.0;
    auto yIndex = size_t(floor(gridHeight * yFract));

    auto xFract = (x - minx) / (maxx - minx);
    if(xFract < 0.0) xFract = 0.0;
    if(xFract > 1.0) xFract = 1.0;
    auto xIndex = size_t(floor(gridWidth * xFract));

    bool create = true;
    Layer * layer = static_cast<Layer *>(getLink(zIndex, create));
    if(layer) {
        create = true;
        Row * row = static_cast<Row *>(layer->getLink(yIndex, create));
        if(row) {
            create = true;
            Cell * cell = static_cast<Cell *>(row->getLink(xIndex, create));
            cell->setTimestamp(chrono::high_resolution_clock::now());
            if(create) {
                count++;
            }
        }
    }
}

size_t OccupancyGrid::getCount() {
    return count;
}

void OccupancyGrid::map(OccupancyGrid::Functor &iFunctor) {
    Layer * layer = static_cast<Layer *>(head);
    while (layer) {
        size_t z = layer->getIndex();
        Row * row = static_cast<Row *>(layer->getHead());
        while (row) {
            size_t y = row->getIndex();
            Cell *cell = static_cast<Cell *>(row->getHead());
            while(cell) {
                size_t x = row->getIndex();
                iFunctor(x, y, z);
                cell = static_cast<Cell *>(cell->getNext());
            }
            row = static_cast<Row *>(row->getNext());
        }
        layer = static_cast<Layer *>(layer->getNext());
    }
}

void OccupancyGrid::prune(float iMaxAge) {
    Layer * layer = static_cast<Layer *>(head);
    vector<Layer *> layersToPrune;
    while(layer) {
        layer->prune(iMaxAge);
        if(!layer->getHead()) {
            layersToPrune.push_back(layer);
        }
        layer = static_cast<Layer *>(layer->getNext());
    }

    for(auto layer: layersToPrune) {
        removeLink(layer->getIndex());
    }
}
