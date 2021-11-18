#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "geometry.hpp"
#include "Target.hpp"

class Follower {

	private:

		static const std::vector< glm::vec4 > defaultCoordinates;
		static constexpr float rotationAngleOffset = 0.0005f;
		static constexpr float translationOffset = 0.0005f;

		float rotation;
		float xTranslation;
		float yTranslation;

		// bool animation;

		float liveRotation;
		float livexTranslation;
		float liveyTranslation;

	public:

		std::vector < glm::vec4 > getPoints();
		glm::vec3 getCentroid();
		glm::mat4 getTransformation();

		bool getLiveAnimation();
		void updateAnimation(const Target &);

};