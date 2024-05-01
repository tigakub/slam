#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "mesh.h"

class Triangle: public Mesh<PCVertex, PCVertex::bufferFormat, GL_POINTS> {
    public:
        Triangle();

    protected:
        virtual void initGeometry();
};

#endif
