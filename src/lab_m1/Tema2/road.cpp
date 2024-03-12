#include "lab_m1/Tema2/road.h"
#include <fstream>

using namespace std;

Road::Road() {

	points.push_back(glm::vec3(3.9f, 0, 12));
	points.push_back(glm::vec3(-4, 0, 12));

	int numSegments = 20;
	float step = glm::pi<float>() / numSegments;
	float theta;
	float radius = 7;

	for (int i = numSegments - 1; i >= 0; i--) {
		theta = glm::pi<float>() + i * step;
		points.push_back(glm::vec3(-4 + radius * sin(theta), 0, 5 + radius * cos(theta)));
	}

	points.push_back(glm::vec3(-4, 0, -2));
	points.push_back(glm::vec3(3.9f, 0, -2));

	radius = 3;
	for (int i = 0; i < numSegments; i++) {
		theta = i * step;
		points.push_back(glm::vec3(4 + radius * sin(theta), 0, -5 + radius * cos(theta)));
	}

	points.push_back(glm::vec3(4, 0, -8));
	points.push_back(glm::vec3(-4, 0, -8));

	radius = 2;
	for (int i = numSegments - 1; i >= 0; i--) {
		theta = glm::pi<float>() + i * step;
		points.push_back(glm::vec3(-4 + radius * sin(theta), 0, -10 + radius * cos(theta)));
	}

	points.push_back(glm::vec3(-3.9, 0, -12));
	points.push_back(glm::vec3(4, 0, -12));

	radius = 7;
	for (int i = numSegments - 1; i >= 0; i--) {
		theta =  i * step;
		points.push_back(glm::vec3(4 + radius * sin(theta), 0, -5 + radius * cos(theta)));
	}

	points.push_back(glm::vec3(3.9f, 0, 2));
	points.push_back(glm::vec3(-3.9f, 0, 2));

	radius = 3;
	for (int i = 0; i < numSegments; i++) {
		theta = -glm::pi<float>() + i * step;
		points.push_back(glm::vec3(-4 + radius * sin(theta), 0, 5 + radius * cos(theta)));
	}

	points.push_back(glm::vec3(-4, 0, 8));
	points.push_back(glm::vec3(4, 0, 8));

	radius = 2;
	for (int i = numSegments - 1; i >= 0; i--) {
		theta = i * step;
		points.push_back(glm::vec3(4 + radius * sin(theta), 0, 10 + radius * cos(theta)));
	}

	numberPoints = points.size();
	glm::vec3 p1, p2, dir, p, inter, interdir;
	for (int i = 0; i < numberPoints - 1; i++) {
		p1 = points[i];
		p2 = points[i + 1];
		dir = p2 - p1;
		for (float i = 0; i < 100; i++) {
			inter = p1 + i * dir  * 0.01f;
			interdir = p2 - inter;
			p = glm::normalize(glm::cross(interdir, glm::vec3(0, 1, 0)));
			vertices.push_back(VertexFormat((inter - p * distI) * scaleFactor, glm::vec3(0.5, 0.5, 0.5)));
			vertices.push_back(VertexFormat((inter + p * distE) * scaleFactor, glm::vec3(0.5, 0.5, 0.5)));
			if ((int)i % 10 == 0 && glm::length(dir) >= 5) {
				forest.push_back(new Tree((inter - p * (distI + 0.4f) * scaleFactor), 0.2));
			}
		}
		p = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
		inner.push_back((p1 - p * distI) * scaleFactor);
		outer.push_back((p1 + p * distE) * scaleFactor);
		
	}

	p1 = points[points.size() - 1];
	p2 = points[0];
	dir = p2 - p1;
	for (float i = 0; i < 100; i++) {
		inter = p1 + i * dir * 0.01f;
		interdir = p2 - inter;
		p = glm::normalize(glm::cross(interdir, glm::vec3(0, 1, 0)));
		vertices.push_back(VertexFormat((inter - p * distI) * scaleFactor, glm::vec3(0.5, 0.5, 0.5)));
		vertices.push_back(VertexFormat((inter + p * distE) * scaleFactor, glm::vec3(0.5, 0.5, 0.5)));
		if ((int)i % 10 == 0 && glm::length(dir) >= 5) {
			forest.push_back(new Tree((inter - p * (distI + 0.4f) * scaleFactor), 0.2));
		}
	}
	p = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	inner.push_back((p1 - p * distI) * scaleFactor);
	outer.push_back((p1 + p * distE) * scaleFactor);

	vertices.push_back(VertexFormat(inner[0], glm::vec3(0.5, 0.5, 0.5)));
	vertices.push_back(VertexFormat(outer[0], glm::vec3(0.5, 0.5, 0.5)));

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	road = new Mesh("road");
	road->SetDrawMode(GL_TRIANGLE_STRIP);
	road->InitFromData(vertices, indices);

	for (int i = 0; i < inner.size(); i++) {
		if (!isOnRoad(inner[i] * 1.05f)) {
			forest.push_back(new Tree(inner[i] * 1.05f, 0.2));
		}
	}

	

}

float Road::triangleArea(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	return glm::length(glm::cross(ab, ac)) / 2;
}

bool Road::isInsideTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	float triangle = triangleArea(a, b, c);
	float test = 0;
	test += triangleArea(a, b, p);
	test += triangleArea(a, c, p);
	test += triangleArea(b, c, p);

	if (glm::abs(test - triangle) <= 0.01f) {
		return true;
	}
	return false;
}

bool Road::isOnRoad(glm::vec3 position)
{
	int i;
	position.y = 0;
	for (i = 0; i < numberPoints - 1; i++) {
		if (isInsideTriangle(position, inner[i], outer[i], inner[i + 1])) {
			return true;
		}

		if (isInsideTriangle(position, inner[i + 1], outer[i + 1], outer[i])) {
			return true;
		}
	}

	if (isInsideTriangle(position, inner[i], inner[0], outer[i])) {
		return true;
	}

	if (isInsideTriangle(position, inner[0], outer[0], outer[i])) {
		return true;
	}

	return false;
}
