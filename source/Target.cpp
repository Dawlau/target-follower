#include "../headers/Target.hpp"
#include <cmath>

Target::Target() {

	center = glm::vec2(500.0f, 500.0f);
	ray = 20.0f;

	generatePoints();
	generateTextureCoordinates();
}

void Target::generateTextureCoordinates() {

	glm::vec2 c = glm::vec2(0.5f, 0.5f);

	for(float theta = 0.0f; theta < 360.0f; theta += thetaStride) {

		float radTheta = theta * PI / 180.0f;

		float x = 0.5f * cos(radTheta);
		float y = 0.5f * sin(radTheta);

		textureCoordinates.push_back(glm::vec2(c.x + x, c.y + y));
	}

	// textureCoordinates.push_back(glm::vec2(0.0f, 0.0f));
	// textureCoordinates.push_back(glm::vec2(1.0f, 0.0f));
	// textureCoordinates.push_back(glm::vec2(1.0f, 1.0f));
	// textureCoordinates.push_back(glm::vec2(0.0f, 1.0f));
}

float Target::getRay() const {
	return ray;
}

std::vector < glm::vec2 > Target::getTextureCoordinates() const {

	return textureCoordinates;
}


std::vector <glm::vec2> Target::getPoints() const {

	return points;
}

void Target::generatePoints() {

	points.clear();

	for(float theta = 0.0f; theta < 360.0f; theta += thetaStride) {

		float radTheta = theta * PI / 180.0f;

		float x = ray * cos(radTheta);
		float y = ray * sin(radTheta);

		points.push_back(glm::vec2(center.x + x, center.y + y));
	}
}

void Target::rePosition() {

	ray = genRandom(minRay, maxRay);

	float centerX = genRandom(0.0f + ray, windowWidth - ray);
	float centerY = genRandom(0.0f + ray, windowHeight - ray);

	center = glm::vec2(centerX, centerY);

	generatePoints();
}

glm::vec2 Target::getCenter() const {

	return center;
}