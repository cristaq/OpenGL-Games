#pragma once
#include "components/simple_scene.h"
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "transform2D.h"

class Duck
{
public:
	Duck();
	~Duck();
	void loadMeshes(std::unordered_map<std::string, Mesh*>&);
	glm::mat3 movement(float, glm::ivec2, bool&, bool&);
	bool checkColissionBullet(int, int);
	glm::mat3 fly(float);
	glm::mat3 flapWing(float);
	glm::mat3 spawn();
	void killDuck();
	void escapeDuck();
	float getTime();
	void incrementSpeed();
	void resetSpeed();
	void checkColissionWindow(glm::ivec2);

	Mesh* body, * head, * upWing, * beak, * downWing, * wings;
	float upWingCornerX;
	float upWingCornerY;

	float downWingCornerX;
	float downWingCornerY;

private:
	float cx = 0, cy = 0;
	float bodyH = 150;
	float bodyBase = 50;
	float beakH = 30;
	float beakBase = 20;
	float wingH = 60;
	float wingBase = 30;
	float radius = 30;

	float angle;
	float scaleSign = 1;
	float scale = 1;
	float initialSpeed = 300;
	float speed = initialSpeed;
	float state = 0;
	float time = 0;

	std::vector<VertexFormat> bodyVertices;
	std::vector<unsigned int> bodyIndices;

	std::vector<VertexFormat> headVertices;
	std::vector<unsigned int> headIndices;

	std::vector<VertexFormat> upWingVertices;
	std::vector<unsigned int> upWingIndices;

	std::vector<VertexFormat> downWingVertices;
	std::vector<unsigned int> downWingIndices;

	std::vector<VertexFormat> wingsVertices;
	std::vector<unsigned int> wingsIndices;

	std::vector<VertexFormat> beakVertices;
	std::vector<unsigned int> beakIndices;
};