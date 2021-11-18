#include "../headers/Target.hpp"
#include <cmath>

Target::Target() {

	center = glm::vec2(500.0f, 500.0f);
	ray = 20.0f;

	for(float theta = 0.0f; theta < 360.0f; theta += thetaStride) {

		float radTheta = theta * PI / 180.0f;

		float x = ray * cos(radTheta);
		float y = ray * sin(radTheta);

		points.push_back(glm::vec2(center.x + x, center.y + y));
	}
}



std::vector <glm::vec2> Target::getPoints() const {

	return points;
}

