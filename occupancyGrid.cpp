#include "occupancyGrid.h"

OccupancyGrid::Cell::Cell(size_t iIndex)
: Link(iIndex) { }

OccupancyGrid::Row::Row(size_t iIndex)
: Container(), Link(iIndex) { }

Link * OccupancyGrid::Row::create(size_t iIndex) {
    return new OccupancyGrid::Cell(iIndex);
}

OccupancyGrid::Layer::Layer(size_t iIndex)
: Container(), Link(iIndex) { }

Link * OccupancyGrid::Layer::create(size_t iIndex) {
    return new OccupancyGrid::Layer(iIndex);
}

OccupancyGrid::OccupancyGrid()
: Container(), gridWidth(0), gridHeight(0), gridDepth(0), minx(0.0), maxx(0.0), miny(0.0), maxy(0.0), minz(0.0), maxz(0.0) { }

OccupancyGrid::OccupancyGrid(size_t iGridWidth, size_t iGridHeight, size_t iGridDepth, float iMinX, float iMaxX, float iMinY, float iMaxY, float iMinZ, float iMaxZ)
: Container(), gridWidth(iGridWidth), gridHeight(iGridHeight), gridDepth(iGridDepth), minx(iMinX), maxx(iMaxX), miny(iMinY), maxy(iMaxY), minz(iMinZ), maxz(iMaxZ) { }

OccupancyGrid::OccupancyGrid(OccupancyGrid &&iOther)
: Container(::move(iOther)), gridWidth(iOther.gridWidth), gridHeight(iOther.gridHeight), gridDepth(iOther.gridDepth), minx(iOther.minx), maxx(iOther.maxx), miny(iOther.miny), maxy(iOther.maxy), minz(iOther.minz), maxz(iOther.maxz) {

 }

OccupancyGrid::~OccupancyGrid() { }

Link * OccupancyGrid::create(size_t iIndex) {
    return new OccupancyGrid::Layer(iIndex);
}

void OccupancyGrid::insertCell(float x, float y, float z) {
    auto zFract = (z - minz) / (maxz - minz);
    if(zFract < 0.0) zFract = 0.0;
    if(zFract > 1.0) zFract = 1.0;
    auto zIndex = gridDepth * zFract;

    auto yFract = (y - miny) / (maxy - miny);
    if(yFract < 0.0) yFract = 0.0;
    if(yFract > 1.0) yFract = 1.0;
    auto yIndex = gridHeight * yFract;

    auto xFract = (x - minx) / (maxx - minx);
    if(xFract < 0.0) xFract = 0.0;
    if(xFract > 1.0) xFract = 1.0;
    auto xIndex = gridWidth * xFract;

    bool create = true;
    Layer * layer = static_cast<Layer *>(getLink(zIndex, create));
    if(layer) {
        create = true;
        Row * row = static_cast<Row *>(layer->getLink(yIndex, create));
        if(row) {
            create = true;
            Cell * cell = static_cast<Cell *>(row->getLink(xIndex, create));
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
            }
            row = static_cast<Row *>(row->getNext());
        }
        layer = static_cast<Layer *>(layer->getNext());
    }
}
