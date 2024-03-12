#pragma once
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "components/simple_scene.h"
#include <vector>

class Tree {
public:
	Tree(glm::vec3 position, float scale);
	~Tree();
	Mesh* base;
	Mesh* leaves;
	glm::vec3 position;
	float scale = 0;

private:
	std::vector<VertexFormat> verticesBase;
	std::vector<unsigned int> indicesBase;
	std::vector<VertexFormat> verticesLeaves;
	std::vector<unsigned int> indicesLeaves;
};