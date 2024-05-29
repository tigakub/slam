#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory> // for shared_ptr

#include "mesh.h"

using namespace std;
using namespace glm;

class GeometryBase {
    public:
        GeometryBase();
        virtual ~GeometryBase();

        // virtual void init() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
};

template <class MeshClass>
class Geometry: public GeometryBase {
    protected:
        shared_ptr<MeshClass> mesh;
        GLuint program;

    public:
        Geometry(MeshClass * iMesh, GLuint iProgram = 0)
        : GeometryBase(), mesh(iMesh), program(iProgram) {
            static_assert(is_base_of<MeshBase, MeshClass>::value, "MeshClass not derived from MeshBase");
        }

        Geometry(Geometry<MeshClass> && iOther)
        : GeometryBase(), mesh(::move(iOther.mesh)), program(iOther.program) { iOther.program = 0; }

        virtual ~Geometry() { }

        /*
        virtual void init() {
            if(mesh.get()) mesh->init();
        }
        */

        virtual void setProgram(GLint iProgram) {
            program = iProgram;
        }

        virtual void update() {
            if(mesh.get()) mesh->update();
        }
       
        virtual void draw() {
            if(program && mesh.get()) {
                glUseProgram(program);
                mesh->draw();
            }
        }

};

template <class MeshClass>
class UnmanagedGeometry: public GeometryBase {
    protected:
        MeshClass & mesh;
        GLuint program;

    public:
        UnmanagedGeometry(MeshClass & iMesh, GLuint iProgram)
        : GeometryBase(), mesh(iMesh), program(iProgram) {
            static_assert(is_base_of<MeshBase, MeshClass>::value, "MeshClass not derived from MeshBase");
        }

        UnmanagedGeometry(Geometry<MeshClass> && iOther)
        : GeometryBase(), mesh(::move(iOther.mesh)), program(iOther.program) { iOther.program = 0; }

        virtual ~UnmanagedGeometry() { }

        /*
        virtual void init() {
            mesh.init();
        }
        */

        virtual void setProgram(GLint iProgram) {
            program = iProgram;
        }

        virtual void update() {
            mesh.update();
        }

        virtual void draw() {
            if(program) {
                glUseProgram(program);
                mesh.draw();
            }
        }

};

#endif