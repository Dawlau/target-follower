#include "../headers/Follower.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>

Follower::Follower() {

	points = {
		glm::vec4(100.0f, 100.0f, 0.0f, 1.0f),
		glm::vec4(200.0f, 100.0f, 0.0f, 1.0f),
		glm::vec4(150.0f, 200.0f, 0.0f, 1.0f)
	};
}

void Follower::updatePoints() {

	// std::cout << xTranslation << ' ' << yTranslation << '\n';

	glm::vec3 centroid = getCentroid();
	// std::cout << rotation << '\n';
	// usleep(15000000);

	// glm::mat4 transformation = getTransformation();
	glm::mat4 transformation =  glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
								glm::translate(centroid) *
								glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
								glm::translate(-centroid);


	for(int i = 0; i < points.size(); i++) {

		points[i] = transformation * points[i];
	}
}

std::vector < glm::vec4 > Follower::getPoints() {

	return points;
}

glm::vec3 Follower::getCentroid() {

	glm::vec4 centroid = glm::vec4(0.0f);

	for(const glm::vec4 &point : points) {

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
	// std::cout << rotation << '\n';

	if(liveRotation) {

		glm::mat4 rotationMatrix;

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

		xTranslation += livexTranslation;
		yTranslation += liveyTranslation;
	}

	return transformation;
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
							   points[points.size() - 1]; // C

	glm::vec2 projectionPoint = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
							  	glm::translate(centroid) *
								glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
							   	glm::translate(-centroid) *
							   	((points[0] + points[1]) / 2.0f); // D

	glm::vec2 closestPoint = targetPoints[0];
	for(int i = 1; i < targetPoints.size(); i++) {
		if(glm::distance(referencePoint, closestPoint) > glm::distance(referencePoint, targetPoints[i])) {
			closestPoint = targetPoints[i]; // X
		}
	}

	// glm::vec2 closestPoint = target.getCenter();

	closestTargetPoint = target.getCenter();

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
						points[points.size() - 1];

	glm::vec2 translationVector = translationOffset * (closestPoint - newPeak);

	livexTranslation = translationVector[0];
	liveyTranslation = translationVector[1];

}

bool Follower::detectCollision() {

	glm::vec3 centroid = getCentroid();

	glm::vec2 peak = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
					 glm::translate(centroid) *
					 glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
					 glm::translate(-centroid) *
					 points[points.size() - 1];


	return (glm::distance(peak, closestTargetPoint) <= collisionDistance);
}

void Follower::reInitAnimation() {

	liveRotation = livexTranslation = liveyTranslation = 0;
	xTranslation = yTranslation = 0;
	rotation = 0;
}