#ifndef __GRID_H__
#define __GRID_H__

#include "shaderData.h"
#include "uniformBuffer.h"
#include "box.h"

class Grid: public UniformBuffer {
    protected:
        GridData grid;

    public:
        Grid(GLuint iBindPoint, bool iIsDynamic = false);
        virtual ~Grid();

        GridData &getGridData();

    protected:
        virtual void initData();
        virtual const void *getData() const;
        virtual GLsizei getDataSize() const;
};

#endif