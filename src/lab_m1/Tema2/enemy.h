#pragma once
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <vector>
#include "road.h"

class Enemy {
public:
	Enemy(int currentPoint, float distanceFromOuterEdge, int direction);
	~Enemy();
	void move(Road*, float deltaTimeSeconds);
	glm::vec3 position = glm::vec3(0, 0, 0);
	float angle = 0;

private:
	int currentPoint;
	int orientation;
	float percentage = 0;
	float speed = 1.5f;
	float distanceFromOuterEdge = 0;
};