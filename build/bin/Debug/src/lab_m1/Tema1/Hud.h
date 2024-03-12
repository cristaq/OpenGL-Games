#pragma once
#include "components/simple_scene.h"
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "transform2D.h"

class Hud
{
public:
	Hud(glm::ivec2);
	void loadMeshes();
	Mesh * heart, * bullet, * score, *scoreBar, * grass;

private:
	std::vector<VertexFormat> heartVertices;
	std::vector<unsigned int> heartIndices;
	float heartRadius = 20;

	std::vector<VertexFormat> bulletVertices;
	std::vector<unsigned int> bulletIndices;
	float bulletH = 20;
	float bulletL = 10;

	std::vector<VertexFormat> scoreVertices;
	std::vector<unsigned int> scoreIndices;
	std::vector<VertexFormat> scoreBarVertices;
	std::vector<unsigned int> scoreBarIndices;
	float scoreWidth = 200;

	std::vector<VertexFormat> grassVertices;
	std::vector<unsigned int> grassIndices;
	float grassHeight = 150;

};