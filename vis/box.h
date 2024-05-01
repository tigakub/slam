#ifndef __BOX_H__
#define __BOX_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "vertex.h"
#include "elementBuffer.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "mesh.h"

#include "usedsa.h"
#include "aabb.h"

using namespace std;
using namespace glm;

/*
class Box {
    protected:
        vector<GLuint> elements;
        vector<Vertex> vertices;

        ElementBuffer<GLuint> ebo;
        VertexBuffer<Vertex> vbo;
        VertexArray vao;

    public:
        Box(float iWidth = 100.0f, float iHeight = 100.0f, float iDepth = 100.0f);

        void init();
        void update();
        
        void draw();

        void expand(AABB &ioBoundingBox);
};
*/

class Box: public Mesh<PNCUVertex, PNCUVertex::bufferFormat, GL_TRIANGLES> {
    protected:
        float width, height, depth;

    public:
        Box(float iWidth = 0.1, float iHeight = 0.1, float iDepth = 0.1);

    protected:
        virtual void initGeometry();
}; 

#endif
