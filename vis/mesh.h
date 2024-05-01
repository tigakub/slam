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
        virtual void update() = 0;
        virtual void cleanUp() = 0;
        virtual void draw() = 0;
        virtual void expand(AABB &ioBoundingBox) = 0;

    protected:
        // virtual vector<PCVertex> & getVertexData() = 0;
        // virtual vector<GLuint> & getElementData() = 0;
};

template<class VertexType, const BufferFormat & iBufferFormat, GLenum iPrimitiveType, bool iEboIsFixedSize = false, GLsizei iEboSize = 0, bool iVboIsFixedSize = false, GLsizei iVboSize = 0>
class Mesh: public MeshBase {
    protected:
        ElementBuffer<GLuint, iPrimitiveType, iEboIsFixedSize, iEboSize> ebo;
        VertexBuffer<VertexType, iBufferFormat, iVboIsFixedSize, iVboSize> vbo;

    public:
        Mesh(bool iVboIsDynamic = false, bool iEboIsDynamic = false)
        : MeshBase(), ebo(iEboIsDynamic), vbo(iVboIsDynamic) { }

        virtual void init() {
            initGeometry();
            vao.init(ebo, vbo);
        }
        
        virtual void update() {
            ebo.update();
            vbo.update();
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
        virtual vector<VertexType> & getVertexData() {
            return vbo.getVertices();
        }
        virtual vector<GLuint> & getElementData() {
            return ebo.getElements();
        }

};

#endif
