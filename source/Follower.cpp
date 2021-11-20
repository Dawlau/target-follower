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

	glm::vec3 centroid = getCentroid();

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

	glm::vec2 G = getCentroid(); // G

	glm::vec2 referencePoint = points[points.size() - 1]; // C

	glm::vec2 center = target.getCenter(); // X

	// update rotation

	glm::vec2 GC = referencePoint - G;
	glm::vec2 GX = center - G;

	liveRotation = angleBetweenVectors(GX, GC);

	if(orientationTest(GC, GX) < 0) {
		liveRotation = -liveRotation;
	}

	// update translation

	glm::vec3 centroid = getCentroid();

	glm::vec2 newPeak =	glm::translate(centroid) *
						glm::rotate(liveRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::translate(-centroid) *
						points[points.size() - 1];

	glm::vec2 translationVector = translationOffset * (center - newPeak);

	livexTranslation = translationVector[0];
	liveyTranslation = translationVector[1];

}

bool Follower::detectCollision(const Target &target) {

	glm::vec3 centroid = getCentroid();

	glm::vec2 peak = glm::translate(glm::vec3(xTranslation, yTranslation, 0.0f)) *
					 glm::translate(centroid) *
					 glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
					 glm::translate(-centroid) *
					 points[points.size() - 1];

	glm::vec2 targetCenter = target.getCenter();
	float targetRay = target.getRay();

	float x = peak[0];
	float y = peak[1];

	float cx = targetCenter[0];
	float cy = targetCenter[1];

	return ((x - cx) * (x - cx) + (y - cy) * (y - cy) < targetRay * targetRay);
}

void Follower::reInitAnimation() {

	liveRotation = livexTranslation = liveyTranslation = 0;
	xTranslation = yTranslation = 0;
	rotation = 0;
}