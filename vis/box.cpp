#include "box.h"
#include "shaderData.h"

/*
Box::Box(float iWidth, float iHeight, float iDepth)
: ebo(), vbo(), vao(ebo, &vbo) {
	
    auto halfWidth = iWidth * 0.5f;
    auto halfHeight = iHeight * 0.5f;
    auto halfDepth = iDepth * 0.5f;

	vec3 p0(-halfWidth, -halfHeight,  halfDepth);
	vec3 p1( halfWidth, -halfHeight,  halfDepth);
	vec3 p2( halfWidth, -halfHeight, -halfDepth);
	vec3 p3(-halfWidth, -halfHeight, -halfDepth);
	vec3 p4(-halfWidth,  halfHeight,  halfDepth);
	vec3 p5( halfWidth,  halfHeight,  halfDepth);
	vec3 p6( halfWidth,  halfHeight, -halfDepth);
	vec3 p7(-halfWidth,  halfHeight, -halfDepth);

	vec3 i(1.0f, 0.0f, 0.0f);
	vec3 j(0.0f, 1.0f, 0.0f);
	vec3 k(0.0f, 0.0f, 1.0f);

	// +x
	vbo.addVertex(Vertex(p2, i, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p6, i, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p5, i, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p1, i, vec3(0,1,0), vec2(0,0)));
	// +z
	vbo.addVertex(Vertex(p1,  k, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p5,  k, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p4,  k, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p0,  k, vec3(0,1,0), vec2(0,0)));
	// +y 
	vbo.addVertex(Vertex(p5,  j, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p6,  j, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p7,  j, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p4,  j, vec3(0,1,0), vec2(0,0)));
	// -x
	vbo.addVertex(Vertex(p0, -i, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p4, -i, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p7, -i, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p3, -i, vec3(0,1,0), vec2(0,0)));
	// -z
	vbo.addVertex(Vertex(p3, -k, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p7, -k, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p6, -k, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p2, -k, vec3(0,1,0), vec2(0,0)));
	// -y
	vbo.addVertex(Vertex(p0, -j, vec3(0,0,1), vec2(1,0)));
	vbo.addVertex(Vertex(p3, -j, vec3(1,1,1), vec2(1,1)));
	vbo.addVertex(Vertex(p2, -j, vec3(1,0,0), vec2(0,1)));
	vbo.addVertex(Vertex(p1, -j, vec3(0,1,0), vec2(0,0)));

	for(unsigned int i = 0; i < 6; ++i)
	{
		ebo.addElement(0+i*4);
		ebo.addElement(1+i*4);
		ebo.addElement(2+i*4);
		ebo.addElement(0+i*4);
		ebo.addElement(2+i*4);
		ebo.addElement(3+i*4);
	}
}

void Box::init() {
	vao.init();
	vbo.init();
	ebo.init();
	vbo.unbind();
	ebo.unbind();
	vao.unbind();
}

void Box::update() {
	ebo.update();
	vbo.update();
}

void Box::draw() {
	vao.bind();
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
	vao.unbind();
}

void Box::expand(AABB &ioBoundingBox) {
    for(auto v: vertices) {
        ioBoundingBox.expand(v.position);
    }
}
*/

Box::Box(float iWidth, float iHeight, float iDepth, HalfMode iWHalfMode, HalfMode iHHalfMode, HalfMode iDHalfMode)
: Mesh<PNCUVertex, PNCUVertex::bufferFormat, GL_TRIANGLES>(),
  width(iWidth), height(iHeight), depth(iDepth),
  wHalfMode(iWHalfMode), hHalfMode(iHHalfMode), dHalfMode(iDHalfMode) { }

void Box::initGeometry() {
	auto pHalfWidth = ((wHalfMode == negative) ? 0.0f : (width * 0.5f));
    auto pHalfHeight = ((hHalfMode == negative) ? 0.0f : (height * 0.5f));
    auto pHalfDepth = ((dHalfMode == negative) ? 0.0f : (depth * 0.5f));

	auto nHalfWidth = ((wHalfMode == positive) ? 0.0f : (-width * 0.5f));
    auto nHalfHeight = ((hHalfMode == positive) ? 0.0f : (-height * 0.5f));
    auto nHalfDepth = ((dHalfMode == positive) ? 0.0f : (-depth * 0.5f));

	vec3 p0( nHalfWidth,  nHalfHeight,  pHalfDepth);
	vec3 p1( pHalfWidth,  nHalfHeight,  pHalfDepth);
	vec3 p2( pHalfWidth,  nHalfHeight,  nHalfDepth);
	vec3 p3( nHalfWidth,  nHalfHeight,  nHalfDepth);
	vec3 p4( nHalfWidth,  pHalfHeight,  pHalfDepth);
	vec3 p5( pHalfWidth,  pHalfHeight,  pHalfDepth);
	vec3 p6( pHalfWidth,  pHalfHeight,  nHalfDepth);
	vec3 p7( nHalfWidth,  pHalfHeight,  nHalfDepth);

	vec3 i(1.0f, 0.0f, 0.0f);
	vec3 j(0.0f, 1.0f, 0.0f);
	vec3 k(0.0f, 0.0f, 1.0f);

	// +x
	vbo.addVertex(PNCUVertex(p2, i, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p6, i, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p5, i, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p1, i, vec4(0,1,0,1), vec2(0,0)));
	// +z
	vbo.addVertex(PNCUVertex(p1,  k, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p5,  k, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p4,  k, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p0,  k, vec4(0,1,0,1), vec2(0,0)));
	// +y 
	vbo.addVertex(PNCUVertex(p5,  j, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p6,  j, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p7,  j, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p4,  j, vec4(0,1,0,1), vec2(0,0)));
	// -x
	vbo.addVertex(PNCUVertex(p0, -i, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p4, -i, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p7, -i, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p3, -i, vec4(0,1,0,1), vec2(0,0)));
	// -z
	vbo.addVertex(PNCUVertex(p3, -k, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p7, -k, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p6, -k, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p2, -k, vec4(0,1,0,1), vec2(0,0)));
	// -y
	vbo.addVertex(PNCUVertex(p0, -j, vec4(0,0,1,1), vec2(1,0)));
	vbo.addVertex(PNCUVertex(p3, -j, vec4(1,1,1,1), vec2(1,1)));
	vbo.addVertex(PNCUVertex(p2, -j, vec4(1,0,0,1), vec2(0,1)));
	vbo.addVertex(PNCUVertex(p1, -j, vec4(0,1,0,1), vec2(0,0)));

	for(unsigned int i = 0; i < 6; ++i)
	{
		ebo.addElement(0+i*4);
		ebo.addElement(1+i*4);
		ebo.addElement(2+i*4);
		ebo.addElement(0+i*4);
		ebo.addElement(2+i*4);
		ebo.addElement(3+i*4);
	}
}