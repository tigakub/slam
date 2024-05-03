#ifndef __NODE_H__
#define __NODE_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory> // for shared_ptr

#include "context.h"
#include "geometry.h"

using namespace std;
using namespace glm;

class Node {
    protected:
        mat4 transform;
        vector<shared_ptr<GeometryBase>> geometries;
        vector<shared_ptr<Node>> children;

    public:
        Node();
        Node(Node & iOther);
        virtual ~Node();
                
        virtual const mat4 & getTransform() const;
        virtual void setTransform(const mat4 & iTransform);
        
        virtual void addGeometry(GeometryBase * iGeom);
        virtual void addChild(Node * iNode);

        virtual void update();
        virtual void draw(Context & iContext);
};

#endif