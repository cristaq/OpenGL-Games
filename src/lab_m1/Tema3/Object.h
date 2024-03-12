#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"

class Object
{
public:
	Object(float slopeSize, glm::vec3 slopeCenter);
	Object(int type, glm::vec3 pos);
	~Object();
	int getType();
	glm::vec3 getPos();
	void move(float deltaTimeSeconds, float angle);
	bool collision(glm::vec3 other);

private:
	int type;
	glm::vec3 pos;

};

