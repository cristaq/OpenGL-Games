#include "lab_m1/Tema1/Hud.h"

Hud::Hud(glm::ivec2 resoltuion) {

	//heart

	int numSegments = 30;
	float step = (2 * glm::pi<float>()) / numSegments;
	float theta;

	for (int i = 0; i < numSegments; i++) {
		theta = i * step;
		heartVertices.push_back(VertexFormat
		(glm::vec3(resoltuion.x - 200 + heartRadius * sin(theta), 
			resoltuion.y - 30 + heartRadius * cos(theta), 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)));
		heartIndices.push_back(i);
	}
	
	// bullet
	bulletVertices.push_back(VertexFormat(glm::vec3(resoltuion.x - 18.5 * bulletL, resoltuion.y - 100, 0),
		glm::vec3(0.7, 0.65, 0.26), glm::vec3(0.2, 0.8, 0.6)));
	bulletVertices.push_back(VertexFormat(glm::vec3(resoltuion.x - 17.5 * bulletL, resoltuion.y - 100, 0),
		glm::vec3(0.7, 0.65, 0.26), glm::vec3(0.2, 0.8, 0.6)));
	bulletVertices.push_back(VertexFormat(glm::vec3(resoltuion.x - 17.5 * bulletL, resoltuion.y - 100 + bulletH, 0), 
		glm::vec3(0.7, 0.65, 0.26), glm::vec3(0.2, 0.8, 0.6)));
	bulletVertices.push_back(VertexFormat(glm::vec3(resoltuion.x - 18.5 * bulletL, resoltuion.y - 100 + bulletH, 0),
		glm::vec3(0.7, 0.65, 0.26), glm::vec3(0.2, 0.8, 0.6)));

	bulletIndices.push_back(0);
	bulletIndices.push_back(1);
	bulletIndices.push_back(2);

	bulletIndices.push_back(0);
	bulletIndices.push_back(2);
	bulletIndices.push_back(3);

	// scoreBar

	scoreBarVertices.push_back(VertexFormat(glm::vec3(10, resoltuion.y - 40, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreBarVertices.push_back(VertexFormat(glm::vec3(10 + scoreWidth, resoltuion.y - 40, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreBarVertices.push_back(VertexFormat(glm::vec3(10 + scoreWidth, resoltuion.y - 10, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreBarVertices.push_back(VertexFormat(glm::vec3(10, resoltuion.y - 10, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));

	scoreBarIndices.push_back(0);
	scoreBarIndices.push_back(1);
	scoreBarIndices.push_back(2);
	scoreBarIndices.push_back(3);

	// score

	scoreVertices.push_back(VertexFormat(glm::vec3(10, resoltuion.y - 40, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreVertices.push_back(VertexFormat(glm::vec3(10 + scoreWidth / 25, resoltuion.y - 40, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreVertices.push_back(VertexFormat(glm::vec3(10 + scoreWidth / 25, resoltuion.y - 10, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));
	scoreVertices.push_back(VertexFormat(glm::vec3(10, resoltuion.y - 10, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)));

	scoreIndices.push_back(0);
	scoreIndices.push_back(1);
	scoreIndices.push_back(2);

	scoreIndices.push_back(0);
	scoreIndices.push_back(2);
	scoreIndices.push_back(3);

	// grass

	grassVertices.push_back(VertexFormat(glm::vec3(0, 0, 0),
		glm::vec3(0, 0.8, 0.2), glm::vec3(0.2, 0.8, 0.6)));
	grassVertices.push_back(VertexFormat(glm::vec3(resoltuion.x, 0, 0),
		glm::vec3(0, 0.8, 0.2), glm::vec3(0.2, 0.8, 0.6)));
	grassVertices.push_back(VertexFormat(glm::vec3(resoltuion.x, grassHeight, 0),
		glm::vec3(0, 0.8, 0.2), glm::vec3(0.2, 0.8, 0.6)));
	grassVertices.push_back(VertexFormat(glm::vec3(0, grassHeight, 0),
		glm::vec3(0, 0.8, 0.2), glm::vec3(0.2, 0.8, 0.6)));

	grassIndices.push_back(0);
	grassIndices.push_back(1);
	grassIndices.push_back(2);

	grassIndices.push_back(0);
	grassIndices.push_back(2);
	grassIndices.push_back(3);
}

void Hud::loadMeshes() {
	heart = new Mesh("heart");
	heart->SetDrawMode(GL_TRIANGLE_FAN);
	heart->InitFromData(heartVertices, heartIndices);

	bullet = new Mesh("heart");
	bullet->InitFromData(bulletVertices, bulletIndices);

	scoreBar = new Mesh("ScoreBar");
	scoreBar->SetDrawMode(GL_LINE_LOOP);
	scoreBar->InitFromData(scoreBarVertices, scoreBarIndices);

	score = new Mesh("Score");
	score->InitFromData(scoreVertices, scoreIndices);

	grass = new Mesh("Grass");
	grass->InitFromData(grassVertices, grassIndices);
}