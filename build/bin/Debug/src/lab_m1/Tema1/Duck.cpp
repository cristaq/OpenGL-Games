#include "lab_m1/Tema1/Duck.h"
#include <iostream>

Duck::Duck() {

	upWingCornerX = cx - 3 * bodyH / 4;
	upWingCornerY = cy + bodyBase / 4;

	downWingCornerX = cx - 3 * bodyH / 4;
	downWingCornerY = cy - bodyBase / 4;

	// body

	glm::vec3 bodyColour = glm::vec3(0.3, 0.1, 0); // brown
	bodyVertices.push_back(VertexFormat(glm::vec3(cx - bodyH, cy - bodyBase / 2, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	bodyVertices.push_back(VertexFormat(glm::vec3(cx, cy, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	bodyVertices.push_back(VertexFormat(glm::vec3(cx - bodyH, cy + bodyBase / 2, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));

	bodyIndices.push_back(0);
	bodyIndices.push_back(1);
	bodyIndices.push_back(2);

	// head

	int numSegments = 30;
	float step = (2 * glm::pi<float>()) / numSegments;
	float theta;
	
	for (int i = 0; i < numSegments; i++) {
		theta = i * step;
		headVertices.push_back(VertexFormat
		(glm::vec3(cx + radius * sin(theta), cy + radius * cos(theta), 0), glm::vec3(0.1, 0.3, 0.1), glm::vec3(0.2, 0.8, 0.6)));
		headIndices.push_back(i);
	}

	// wings

	upWingVertices.push_back(VertexFormat(glm::vec3(upWingCornerX, upWingCornerY, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	upWingVertices.push_back(VertexFormat(glm::vec3(upWingCornerX + wingBase, upWingCornerY, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	upWingVertices.push_back(VertexFormat(glm::vec3(upWingCornerX + wingBase / 2, upWingCornerY + wingH, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));

	upWingIndices.push_back(0);
	upWingIndices.push_back(1);
	upWingIndices.push_back(2);

	downWingVertices.push_back(VertexFormat
	(glm::vec3(downWingCornerX, downWingCornerY, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	downWingVertices.push_back(VertexFormat
	(glm::vec3(downWingCornerX + wingBase, downWingCornerY, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	downWingVertices.push_back(VertexFormat
	(glm::vec3(downWingCornerX + wingBase / 2, downWingCornerY - wingH, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));

	downWingIndices.push_back(0);
	downWingIndices.push_back(1);
	downWingIndices.push_back(2);

	wingsVertices.push_back(VertexFormat(glm::vec3(cx - 4 * bodyH / 6, cy, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	wingsVertices.push_back(VertexFormat(glm::vec3(cx - bodyH / 2, cy - wingH, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	wingsVertices.push_back(VertexFormat(glm::vec3(cx - bodyH / 3, cy , 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));
	wingsVertices.push_back(VertexFormat(glm::vec3(cx - bodyH / 2, cy + wingH, 0), bodyColour, glm::vec3(0.2, 0.8, 0.6)));

	wingsIndices.push_back(0);
	wingsIndices.push_back(1);
	wingsIndices.push_back(2);

	wingsIndices.push_back(0);
	wingsIndices.push_back(2);
	wingsIndices.push_back(3);


	// beak

	beakVertices.push_back(VertexFormat(glm::vec3(cx + radius, cy + beakBase / 2, 0), glm::vec3(1, 0.8, 0.1), glm::vec3(0.2, 0.8, 0.6)));
	beakVertices.push_back(VertexFormat(glm::vec3(cx + radius, cy - beakBase / 2, 0), glm::vec3(1, 0.8, 0.1), glm::vec3(0.2, 0.8, 0.6)));
	beakVertices.push_back(VertexFormat(glm::vec3(cx + radius + beakH, cy, 0), glm::vec3(1, 0.8, 0.1), glm::vec3(0.2, 0.8, 0.6)));

	beakIndices.push_back(0);
	beakIndices.push_back(1);
	beakIndices.push_back(2);

}

void Duck::loadMeshes(std::unordered_map<std::string, Mesh*>& meshes) {
	body = new Mesh("body");
	body->InitFromData(bodyVertices, bodyIndices);
	upWing = new Mesh("upwing");
	upWing->InitFromData(upWingVertices, upWingIndices);
	downWing = new Mesh("downwing");
	downWing->InitFromData(downWingVertices, downWingIndices);
	head = new Mesh("head");
	head->SetDrawMode(GL_TRIANGLE_FAN);
	head->InitFromData(headVertices, headIndices);
	beak = new Mesh("beak");
	beak ->InitFromData(beakVertices, beakIndices);
	wings = new Mesh("wings");
	wings->InitFromData(wingsVertices, wingsIndices);
}

glm::mat3 Duck::movement(float deltaTimeSeconds, glm::ivec2 resolution, bool &escaped, bool& dead)
{
	glm::mat3 modelMatrix(1);
	if (state == 0) {
		modelMatrix = spawn();
		state = 1;
		time = 0;
	}
	else if (state == 1) {
		time += deltaTimeSeconds;
		checkColissionWindow(resolution);
		modelMatrix = fly(deltaTimeSeconds);
	}
	else if (state == 2) {
		escaped = true;
		angle = glm::pi<float>() / 2;
		modelMatrix = fly(deltaTimeSeconds);
		if (cy > resolution.y + radius + bodyH) {
			state = 0;
			escaped = false;
		}
	}
	else if (state == 3) {
		dead = true;
		angle = -1 * glm::pi<float>() / 2;
		modelMatrix = fly(deltaTimeSeconds);
		if (cy < 150 - radius - bodyH) {
			state = 0;
			dead = false;
		}
	}
	return modelMatrix;
}

bool Duck::checkColissionBullet(int mouseX, int mouseY)
{
	float centerX = cx + bodyH / 2 * cos(angle);
	float centerY = cy + bodyH / 2 * sin(angle);
	float a = bodyH + radius + beakH;
	float b = wingH;
	float coss = cos(angle);
	float ssin = sin(angle);
	
	if ((pow((coss * (mouseX - centerX) + ssin * (mouseY - centerY)), 2) / pow(a, 2) +
		pow((ssin * (mouseX - centerX) - coss * (mouseY - centerY)), 2) / pow(b, 2)) <= 1) {
		return true;
	}

	return false;
}

glm::mat3 Duck::fly(float deltaTime)
{
	glm::mat3 modelMatrix(1);
	cx += deltaTime * cos(angle) * speed;
	cy += deltaTime * sin(angle) * speed;
	modelMatrix *= transform2D::Translate(cx, cy);
	modelMatrix *= transform2D::Rotate(angle);

	return modelMatrix;
}

glm::mat3 Duck::flapWing(float deltaTimeSeconds)
{
	glm::mat3 modelMatrix = glm::mat3(1);

	
	modelMatrix *= transform2D::Translate(cx, cy);
	modelMatrix *= transform2D::Rotate(angle);
	modelMatrix *= transform2D::Translate(-1 * bodyH / 2, 0);
	if (scale > 1) {
		scaleSign = -1;
	}
	else if (scale < 0) {
		scaleSign = 1;
	}
	scale += deltaTimeSeconds * scaleSign * 2;
	modelMatrix *= transform2D::Scale(scale, scale);
	modelMatrix *= transform2D::Translate(bodyH / 2, 0);
	return modelMatrix;
}

glm::mat3 Duck::spawn()
{
	angle = 30 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * glm::pi<float>()) - 30));
	cx = 200 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1000));
	cy = 200 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 300));
	glm::mat3 modelMatrix(1);
	modelMatrix *= transform2D::Translate(cx, cy);
	modelMatrix *= transform2D::Rotate(angle);
	return modelMatrix;
}

void Duck::killDuck()
{
	state = 3;
}

void Duck::escapeDuck()
{
	state = 2;
	time = 0;
}

float Duck::getTime()
{
	return time;
}

void Duck::incrementSpeed()
{
	speed += speed / 2;
}

void Duck::resetSpeed() {
	speed = initialSpeed;
}

void Duck::checkColissionWindow(glm::ivec2 resolution)
{
	float beakX = cx + (beakH + radius) * cos(angle);
	float beakY = cy + (beakH + radius) * sin(angle);

	if (beakX < 0 || beakX > resolution.x) {
		angle = glm::pi<float>() - angle;
	}
	else if (beakY < 150 || beakY > resolution.y) {
		angle = 2 * glm::pi<float>() - angle;
	}
}


