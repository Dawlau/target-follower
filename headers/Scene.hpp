#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Target.hpp"
#include "Follower.hpp"
#include "geometry.hpp"

class Scene {

private:

	Scene() {

		normMatrix = glm::scale(glm::vec3(2.0f / windowWidth, 2.0f / windowHeight, 1.0f)) * glm::translate(glm::vec3(-windowWidth / 2.0f, -windowHeight / 2.0f, 0.0f));
	}

	Target target;
	Follower follower;

	static const int followerPointsCount = 3;
	static const int targetPointsCount = 360;

	static const int vertexSize = 9;
	static const int vertexCount = followerPointsCount + targetPointsCount;

	static const int initialWindowsPosX = 100;
	static const int initialWindowsPosY = 100;

	static const int windowWidth = 800;
	static const int windowHeight = 600;


	glm::mat4 normMatrix;


	const char* vertexShaderPath = "shaders/shader.vert";
	const char* fragmentShaderPath = "shaders/shader.frag";


	GLuint vaoId;
	GLuint vboId;
	GLuint shadersId;
	GLuint texture;

	GLfloat vertices[vertexSize * vertexCount] = {

		// coords						// colors					// texture coords
		100.0f, 100.0f, 0.0f, 1.0f, 	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		200.0f, 100.0f, 0.0f, 1.0f, 	1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // follower
		150.0f, 200.0f, 0.0f, 1.0f, 	1.0f, 0.0f, 0.0f,		0.5f, 1.0f,
		// 500.0f, 500.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f, // target
	};




	void createVBO();
	void destroyVBO();

	void loadShaders();
	void unloadShaders();

	void loadTexture();

public:

	Scene(Scene const&)            = delete;
    void operator =(Scene const&)  = delete;

	void setup();
	void run(int, char**);

	void cleanup();
	void render();

	static Scene& getInstance();
};

void renderWrapper();
void cleanupWrapper();