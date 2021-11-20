#ifndef FOLLOWER_H
#define FOLLOWER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "geometry.hpp"
#include "Target.hpp"

class Follower {

	private:

		static constexpr float rotationAngleOffset = 0.0005f;
		static constexpr float translationOffset = 0.0005f;
		static constexpr float collisionDistance = 0.1f;

		std::vector< glm::vec4 > points;

		float rotation;
		float xTranslation;
		float yTranslation;

		float liveRotation;
		float livexTranslation;
		float liveyTranslation;

	public:

		Follower();

		glm::vec3 getCentroid();
		glm::mat4 getTransformation();

		bool getLiveAnimation();
		void updateAnimation(const Target &);

		bool detectCollision(const Target &);
		void reInitAnimation();

		void updatePoints();
		std::vector < glm::vec4 > getPoints();
};

#endif