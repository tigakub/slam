#include "node.h"

Node::Node()
: transform(1.0f), geometries(), children() { }

Node::Node(Node & iOther)
: transform(iOther.transform), geometries(iOther.geometries), children(iOther.children) { }

Node::~Node() {  }

const mat4 & Node::getTransform() const {
    return transform;
}

void Node::setTransform(const mat4 & iTransform) {
    transform = iTransform;
}

void Node::addGeometry(GeometryBase * iGeom) {
    geometries.push_back(shared_ptr<GeometryBase>(iGeom));
}

void Node::addChild(Node * iNode) {
    children.push_back(shared_ptr<Node>(iNode));
}

void Node::update() {
    for(auto geom: geometries) {
        geom->update();
    }

    for(auto child: children) {
        child->update();
    }
}

void Node::draw(Context & iContext) {
    iContext.pushMatrix(transform);

    for(auto geom: geometries) {
        geom->draw();
    }

    for(auto child: children) {
        child->draw(iContext);
    }

    iContext.popMatrix();
}
