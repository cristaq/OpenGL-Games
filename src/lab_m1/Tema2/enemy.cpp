#include "enemy.h"

Enemy::Enemy(int start, float dist, int ori) {
	currentPoint = start;
	distanceFromOuterEdge = dist;
	orientation = ori;
}

Enemy::~Enemy()
{
}

void Enemy::move(Road* road, float deltaTime) {
	glm::vec3 start;
	glm::vec3 end;

	if (percentage >= 1 && orientation == 1) {
		currentPoint++;
		percentage = 0;
		if (currentPoint == road->numberPoints) {
			currentPoint = 0;
		}
	}
	else if (percentage >= 1 && orientation == -1) {
		currentPoint--;
		percentage = 0;
		if (currentPoint == -1) {
			currentPoint = road->numberPoints - 1;
		}
	}

	if (currentPoint == road->numberPoints - 1 && orientation == 1) {
		start = road->outer[currentPoint] + distanceFromOuterEdge * (road->inner[currentPoint] - road->outer[currentPoint]);
		end = road->outer[0] + distanceFromOuterEdge * (road->inner[0] - road->outer[0]);
	}
	else if (currentPoint == 0 && orientation == -1) {
		start = road->outer[currentPoint] + distanceFromOuterEdge * (road->inner[currentPoint] - road->outer[currentPoint]);
		end = road->outer[road->numberPoints - 1] + distanceFromOuterEdge * (road->inner[road->numberPoints - 1] - road->outer[road->numberPoints - 1]);
	}
	else if(orientation == 1) {
		start = road->outer[currentPoint] + distanceFromOuterEdge * (road->inner[currentPoint] - road->outer[currentPoint]);
		end = road->outer[currentPoint + 1] + distanceFromOuterEdge * (road->inner[currentPoint + 1] - road->outer[currentPoint + 1]);
	}
	else if (orientation == -1) {
		start = road->outer[currentPoint] + distanceFromOuterEdge * (road->inner[currentPoint] - road->outer[currentPoint]);
		end = road->outer[currentPoint - 1] + distanceFromOuterEdge * (road->inner[currentPoint - 1] - road->outer[currentPoint - 1]);
	}
	glm::vec3 dir = end - start;

	percentage += deltaTime * speed / glm::length(dir);
	position = start + dir * percentage;

	if (dir.z == 0) {
		angle = glm::sign(dir.x) * glm::pi<float>() / 2;
	}
	else if ((dir.z > 0 && dir.x > 0) || (dir.z > 0 && dir.x < 0)) {
		angle = glm::atan(dir.x / dir.z);
	}
	else {
		angle = glm::atan(dir.x / dir.z) + glm::pi<float>();
	}
}
