#include "grid.h"

Grid::Grid(Box & iGridBox, GLuint iInstanceDataBindPoint, bool iIsDynamic, GLuint iShaderProgram)
: InstanceCloud<InstanceData, Box, false, 0>(iGridBox, iInstanceDataBindPoint, iIsDynamic, iShaderProgram) {
}

void Grid::update() {
    InstanceCloud<InstanceData, Box, false, 0>::update();
}

void Grid::draw() {
    InstanceCloud<InstanceData, Box, false, 0>::draw();
}
