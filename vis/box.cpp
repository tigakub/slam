#include "box.h"

Box::Box(float iWidth, float iHeight, float iDepth) {
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
	vertices.push_back({p2, i, {0,0,1}, {1,0}});
	vertices.push_back({p6, i, {1,1,1}, {1,1}});
	vertices.push_back({p5, i, {1,0,0}, {0,1}});
	vertices.push_back({p1, i, {0,1,0}, {0,0}});
	// +z
	vertices.push_back({p1, k, {0,0,1}, {1,0}});
	vertices.push_back({p5, k, {1,1,1}, {1,1}});
	vertices.push_back({p4, k, {1,0,0}, {0,1}});
	vertices.push_back({p0, k, {0,1,0}, {0,0}});
	// +y
	vertices.push_back({p5, j, {0,0,1}, {1,0}});
	vertices.push_back({p6, j, {1,1,1}, {1,1}});
	vertices.push_back({p7, j, {1,0,0}, {0,1}});
	vertices.push_back({p4, j, {0,1,0}, {0,0}});
	// -x
	vertices.push_back({p0, -i, {0,0,1}, {1,0}});
	vertices.push_back({p4, -i, {1,1,1}, {1,1}});
	vertices.push_back({p7, -i, {1,0,0}, {0,1}});
	vertices.push_back({p3, -i, {0,1,0}, {0,0}});
	// -z
	vertices.push_back({p3, -k, {0,0,1}, {1,0}});
	vertices.push_back({p7, -k, {1,1,1}, {1,1}});
	vertices.push_back({p6, -k, {1,0,0}, {0,1}});
	vertices.push_back({p2, -k, {0,1,0}, {0,0}});
	// -y
	vertices.push_back({p0, -j, {0,0,1}, {1,0}});
	vertices.push_back({p3, -j, {1,1,1}, {1,1}});
	vertices.push_back({p2, -j, {1,0,0}, {0,1}});
	vertices.push_back({p1, -j, {0,1,0}, {0,0}});

	for(unsigned int i = 0; i < 6; ++i)
	{
		elements.push_back(0+i*4);
		elements.push_back(1+i*4);
		elements.push_back(2+i*4);
		elements.push_back(0+i*4);
		elements.push_back(2+i*4);
		elements.push_back(3+i*4);
	}
}
