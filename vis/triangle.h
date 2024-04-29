#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

#include "mesh.h"

class Triangle: public Mesh<PCVertex, PCVertex::bufferFormat, GL_TRIANGLES> {
    public:
        Triangle();

    protected:
        virtual void initGeometry();
};

#endif
