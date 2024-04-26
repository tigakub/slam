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

OccupancyGrid::OccupancyGrid(size_t iGridWidth, size_t iGridHeight, size_t iGridDepth, float iMinX, float iMaxX, float iMinY, float iMaxY, float iMinZ, float iMaxZ)
: Container(), gridWidth(iGridWidth), gridHeight(iGridHeight), gridDepth(iGridDepth), minx(iMinX), maxx(iMaxX), miny(iMinY), maxy(iMaxY), minz(iMinZ), maxz(iMaxZ) { }

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

    Layer * layer = static_cast<Layer *>(getLink(zIndex, true));
    if(layer) {
        Row * row = static_cast<Row *>(layer->getLink(yIndex, true));
        if(row) {
            Cell * cell = static_cast<Cell *>(row->getLink(xIndex, true));
        }
    }
}