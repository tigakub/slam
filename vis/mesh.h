#ifndef __MESH_H__
#define __MESH_H__

#include "aabb.h"
#include "vertex.h"
#include "vertexArray.h"
#include "elementBuffer.h"
#include "vertexBuffer.h"

class MeshBase {
    protected:
        VertexArray vao;

    public:
        MeshBase(): vao() { };

        virtual void init() = 0;
        virtual void cleanUp() = 0;
        virtual void draw() = 0;
        virtual void expand(AABB &ioBoundingBox) = 0;
};

template<class VertexType, const BufferFormat & iBufferFormat, GLenum iPrimitiveType>
class Mesh: public MeshBase {
    protected:
        ElementBuffer<GLuint, iPrimitiveType> ebo;
        VertexBuffer<VertexType, iBufferFormat> vbo;

    public:
        Mesh()
        : MeshBase(), ebo(), vbo() { }

        virtual void init() {
            initGeometry();
            vao.init(ebo, vbo);
        }

        virtual void cleanUp() {
            ebo.cleanUp();
            vbo.cleanUp();
        }
        
        virtual void draw() {
            vao.bind();
            // glDrawArrays(ebo.getPrimitiveType(), 0, vbo.getCount());
            glDrawElements(ebo.getPrimitiveType(), ebo.getCount(), GL_UNSIGNED_INT, 0);
            vao.unbind();
        }

        // This does not take into account model transformations.
        virtual void expand(AABB &ioBoundingBox) {
            for(auto vertex: vbo.getVertices()) {
            }
        }

    protected:
        virtual void initGeometry() = 0;

};

#endif
