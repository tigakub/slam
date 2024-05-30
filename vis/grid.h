#ifndef __GRID_H__
#define __GRID_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderData.h"
#include "uniformBuffer.h"
#include "instanceCloud.h"
#include "box.h"

class Grid: public InstanceCloud<InstanceData, Box, false, 0> {
    public:
        typedef ShaderStorageBuffer<InstanceData, PointScaleVertex::bufferFormat, false> StorageBuffer;
        
    public:
        Grid(Box & iGridBox, GLuint iInstanceDataBindPoint = 4, bool iIsDynamic = false, GLuint iShaderProgram = 0);

        virtual void update();
        virtual void draw();
};

#endif