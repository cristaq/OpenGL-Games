#pragma once
#include "components/simple_scene.h"
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>
#include "tree.h"

class Road {
public:
	Road();
	~Road();
	Mesh* road;
	bool isOnRoad(glm::vec3 position);
	float scaleFactor = 1;
	std::vector<Tree*> forest;

private:
	std::vector<VertexFormat> vertices;
	std::vector<glm::vec3> points;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> outer;
	std::vector<glm::vec3> inner;
	float triangleArea(glm::vec3, glm::vec3, glm::vec3);
	bool isInsideTriangle(glm::vec3, glm::vec3, glm::vec3, glm::vec3);

	int numberPoints = 0;
	float distI = 0.5f;
	float distE = 1.5f;

	friend class Enemy;
};
