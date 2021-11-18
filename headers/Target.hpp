#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include "geometry.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Target {

	private:

		std::vector < glm::vec2 > points;
		float ray;
		glm::vec2 center;

		static constexpr float thetaStride = 1.0f;
		static constexpr float minRay = 15.0f;
		static constexpr float maxRay = 25.0f;


		static const int windowWidth = 800;
		static const int windowHeight = 600;



	public:

		Target();

		std::vector <glm::vec2> getPoints() const;
		void rePosition();
};

#endif