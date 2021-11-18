#include "../headers/Follower.hpp"
#include <iostream>
#include <iomanip>

const std::vector < glm::vec4 > Follower::defaultCoordinates = {

	glm::vec4(100.0f, 100.0f, 0.0f, 1.0f),
	glm::vec4(200.0f, 100.0f, 0.0f, 1.0f),
	glm::vec4(150.0f, 200.0f, 0.0f, 1.0f),
};

glm::vec3 Follower::getCentroid() {

	glm::vec4 centroid = glm::vec4(0.0f);

	for(const glm::vec4 &point : defaultCoordinates) {

		centroid += point;
	}

	return centroid / 3.0f;
}

glm::mat4 Follower::getTransformation() {

	glm::vec3 centroid = getCentroid();

	glm::mat4 transformation =  glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
								glm::translate(centroid) *
								glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
								glm::translate(-centroid);
;

	if(liveRotation) {

		glm::mat4 rotationMatrix;

		// rotation += rotationAngleOffset;


		if(fabs(liveRotation) < rotationAngleOffset) {

			rotationMatrix = glm::rotate(-liveRotation, glm::vec3(0.0f, 0.0f, 1.0f));
			rotation += liveRotation;
			liveRotation = 0;
		}
		else {
			if (liveRotation < 0) {
				rotationMatrix = glm::rotate(-rotationAngleOffset, glm::vec3(0.0f, 0.0f, 1.0f));
				rotation -= rotationAngleOffset;
				liveRotation += rotationAngleOffset;
			}
			else {
				rotationMatrix = glm::rotate(rotationAngleOffset, glm::vec3(0.0f, 0.0f, 1.0f));
				rotation += rotationAngleOffset;
				liveRotation -= rotationAngleOffset;
			}
		}

		transformation = 	glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
							glm::translate(centroid) *
							rotationMatrix *
							glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
							glm::translate(-centroid);
	}
	else if(livexTranslation or liveyTranslation) {
		std::cout << "in\n";
		transformation = glm::translate(glm::vec3(livexTranslation, liveyTranslation, 0.0f)) * transformation;
		xTranslation += livexTranslation;
		yTranslation += liveyTranslation;
	}





	// glm::mat4 transformation = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
	// 						   glm::translate(centroid) *
	// 						   glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
	// 						   glm::translate(-centroid);

	return transformation;
}

std::vector < glm::vec4 > Follower::getPoints() {

	std::vector < glm::vec4 > points;
	points.reserve(defaultCoordinates.size());

	for(int i = 0; i < defaultCoordinates.size(); ++i) {

		glm::vec4 point = defaultCoordinates[i];
		points.push_back(getTransformation() * point);
	}

	return points;
}

bool Follower::getLiveAnimation() {

	return (liveRotation or livexTranslation or liveyTranslation);
}

void Follower::updateAnimation(const Target& target) {

	std::vector < glm::vec2 > targetPoints = target.getPoints();

	glm::vec3 centroid = getCentroid();
	glm::vec2 referencePoint = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
							   glm::translate(centroid) *
							   glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
							   glm::translate(-centroid) *
							   defaultCoordinates[defaultCoordinates.size() - 1]; // C

	glm::vec2 projectionPoint = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
							  	glm::translate(centroid) *
								glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
							   	glm::translate(-centroid) *
							   	((defaultCoordinates[0] + defaultCoordinates[1]) / 2.0f); // D

	glm::vec2 closestPoint = targetPoints[0];
	for(int i = 1; i < targetPoints.size(); i++) {
		if(glm::distance(referencePoint, closestPoint) > glm::distance(referencePoint, targetPoints[i])) {
			closestPoint = targetPoints[i]; // X
		}
	}

	// update rotation

	glm::vec2 CD = projectionPoint - referencePoint;
	glm::vec2 CX = closestPoint - referencePoint;

	liveRotation = PI - angleBetweenVectors(CD, CX);

	if(orientationTest(CD, CX) > 0) {
		liveRotation = -liveRotation;
	}


	// update translation

	glm::vec2 newPeak = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
			  			glm::translate(centroid) *
						glm::rotate(liveRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			  			glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			  			glm::translate(-centroid) *
						defaultCoordinates[defaultCoordinates.size() - 1];

	glm::vec2 translationVector = translationOffset * (closestPoint - newPeak);

	livexTranslation = translationVector[0];
	liveyTranslation = translationVector[1];

	std::cout << livexTranslation << ' ' << liveyTranslation << '\n';
}