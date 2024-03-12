#include "Object.h"

Object::Object(float slopeSize, glm::vec3 slopeCenter)
{
	type = rand() % 4;
	pos.y = 0;
	float low = -1 * slopeCenter.x + slopeSize / 2;
	pos.x = low + static_cast <float> (rand()) / 
		( static_cast <float> (RAND_MAX/(-1 * low -low)));

	low = slopeSize - slopeSize / 4;
	float high = slopeSize - slopeSize / 5;
	pos.z = low + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (high - low)));
}

Object::Object(int type, glm::vec3 pos)
{
	this->type = type;
	this->pos = pos;
}

Object::~Object()
{
}

int Object::getType()
{
	return type;
}

glm::vec3 Object::getPos()
{
	return pos;
}

void Object::move(float deltaTimeSeconds, float angle)
{
	pos.z -= deltaTimeSeconds * 2 * cos(angle);
	pos.x -= sin(angle) * deltaTimeSeconds * 3;
}

bool Object::collision(glm::vec3 other)
{
	float distance;
	distance = sqrt((pos.x - other.x) * (pos.x - other.x) +
		(pos.y - other.y) * (pos.y - other.y) +
		(pos.z - other.z) * (pos.z - other.z)
	);
	return distance < 0.5;
}
