#include "../headers/geometry.hpp"

float orientationTest(glm::vec2 a, glm::vec2 b) {

	/*

	< 0 => a on the right
	> 0 => b on the left

	*/

	glm::mat3 orientationMatrix = {

		a.x, a.y, 1.0f,
		b.x, b.y, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	return glm::determinant(orientationMatrix);
}

float angleBetweenVectors(glm::vec2 a, glm::vec2 b) {

	float dotProduct = glm::dot(a, b);
	float lengthA = glm::length(a);
	float lengthB = glm::length(b);

	float angle = acos(dotProduct / (lengthA * lengthB));
	return angle;
}