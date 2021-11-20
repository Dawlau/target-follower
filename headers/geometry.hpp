#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <random>

static constexpr float PI = 3.14159265f;

float orientationTest(glm::vec2, glm::vec2);
float angleBetweenVectors(glm::vec2, glm::vec2);
float genRandom(const float , const float );

#endif