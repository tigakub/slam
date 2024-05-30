#include "box.h"
#include "shaderData.h"

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