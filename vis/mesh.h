#ifndef __MESH_H__
#define __MESH_H__

#include "aabb.h"
#include "vertex.h"
#include "vertexArray.h"
#include "elementBuffer.h"
#include "vertexBuffer.h"

class MeshBase {
    protected:
        bool isNew;

    public:
        MeshBase(): isNew(true) { }
        virtual ~MeshBase() { }

        virtual ElementBufferBase & getEBO() = 0;
        virtual VertexBufferBase & getVBO() = 0;

        //virtual void init() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void expand(AABB &ioBoundingBox) = 0;

    protected:
        // virtual vector<PCVertex> & getVertexData() = 0;
        // virtual vector<GLuint> & getElementData() = 0;
};

template<class VertexType, const BufferFormat & iBufferFormat, GLenum iPrimitiveType, bool iEboIsFixedSize = false, GLsizei iEboSize = 0, bool iVboIsFixedSize = false, GLsizei iVboSize = 0>
class Mesh: public MeshBase {
    protected:
        VertexArray vao;
        ElementBuffer<GLuint, iPrimitiveType, iEboIsFixedSize, iEboSize> ebo;
        VertexBuffer<VertexType, iBufferFormat, iVboIsFixedSize, iVboSize> vbo;

    public:
        Mesh(bool iVboIsDynamic = false, bool iEboIsDynamic = false)
        : MeshBase(), vao(), ebo(iEboIsDynamic), vbo(iVboIsDynamic) { }

        virtual ~Mesh() { }

        /*
        virtual void init() {
            initGeometry();
            vao.init(ebo, vbo);
        }
        */

        virtual ElementBufferBase & getEBO() {
            return ebo;
        }

        virtual VertexBufferBase & getVBO() {
            return vbo;
        }
        
        virtual void update() {
            if(isNew) {
                initGeometry();
                vao.init(ebo, vbo);
                vao.unbind();
                isNew = false;
            } else {
                ebo.update();
                vbo.update();
            }
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
