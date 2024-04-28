#include "box.h"
#include "shaderData.h"

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
	ebo.init();
	vbo.init();
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
