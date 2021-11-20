#include "../headers/Scene.hpp"
#include "../headers/loadShaders.h"
#include "../headers/SOIL.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>

Scene& Scene::getInstance() {

	static Scene instance;
	return instance;
}

void Scene::createVBO() {

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

}

void Scene::destroyVBO() {

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vaoId);

}

void Scene::loadShaders() {

	shadersId = LoadShaders(vertexShaderPath, fragmentShaderPath);
  	glUseProgram(shadersId);
}

void Scene::unloadShaders() {

	glDeleteProgram(shadersId);
}

void Scene::loadTexture(const char *imagePath, GLuint &texture) {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);

    if (image) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, texture);

    }
    else {
        std::cout << "Error loading the texture\n";
    }
}

void Scene::cleanup() {

	destroyVBO();
	unloadShaders();
}

void Scene::setup() {

	glClearColor(1.0f, 1.0f, 0.0f, 0.0f); // white background
    updateTargetCoordinates();
    updateFollowerCoordinates();

    loadTexture(targetTexturePath, targetTexture);
    loadTexture(followerTexturePath, followerTexture);

    loadShaders();

}

void Scene::updateTargetCoordinates() {

    std::vector < glm::vec2 > targetPoints = target.getPoints();
    std::vector < glm::vec2 > textureCoordinates = target.getTextureCoordinates();

    // std::cout << targetPoints.size() << '\n';

    for(int i = 0; i < targetPoints.size(); ++i) {

        // std::cout << targetPoints[i][0] << ' ' << targetPoints[i][1] << '\n';

        int idx = (i + followerPointsCount) * vertexSize; // add 3 because the first 3 points in the vbo are follower points

        vertices[idx] = targetPoints[i][0];
        vertices[idx + 1] = targetPoints[i][1]; // coordinates
        vertices[idx + 2] = 0.0f;
        vertices[idx + 3] = 1.0f;

        vertices[idx + 4] = 0.0f;
        vertices[idx + 5] = 0.0f; // colors
        vertices[idx + 6] = 0.0f;

        vertices[idx + 7] = textureCoordinates[i][0]; // texture coordinates
        vertices[idx + 8] = textureCoordinates[i][1];
    }
}

void Scene::updateFollowerCoordinates() {

    std::vector < glm::vec4 > followerPoints = follower.getPoints();

    for(int i = 0; i < followerPoints.size(); i++) {

        int idx = i * vertexSize;

        vertices[idx] = followerPoints[i][0];
        vertices[idx + 1] = followerPoints[i][1];
        vertices[idx + 2] = 0.0f;
        vertices[idx + 3] = 1.0f;

        vertices[idx + 4] = 0.0f;
        vertices[idx + 5] = 0.0f;
        vertices[idx + 6] = 0.0f;

        if(i == 0){
        vertices[idx + 7] = 0.0f;
        vertices[idx + 8] = 1.0f;
        }
        else if(i == 1) {
            vertices[idx + 7] = 0.0f;
            vertices[idx + 8] = 0.0f;
        }
        else if(i == 2) {
            vertices[idx + 7] = 1.0f;
            vertices[idx + 8] = 0.5f;
        }
    }
}

void Scene::render() {

	glClear(GL_COLOR_BUFFER_BIT);

    // std::cout << targetTexture << ' ' << followerTexture << '\n';

    createVBO();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, targetTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, followerTexture);

    if (follower.detectCollision(target)) {

        follower.updatePoints();
        follower.reInitAnimation();
        target.rePosition();
        updateTargetCoordinates();
        updateFollowerCoordinates();
    }

    if (!follower.getLiveAnimation()){

        follower.updateAnimation(target);
    }



    glm::mat4 followerTransformation = follower.getTransformation();
    followerTransformation = normMatrix * followerTransformation;

    // render follower
    GLuint normMatrixLoc = glGetUniformLocation(shadersId, "normMatrix");
    glUniformMatrix4fv(normMatrixLoc, 1, GL_FALSE, &followerTransformation[0][0]);
    glUniform1i(glGetUniformLocation(shadersId, "target"), 0);

    glUniform1i(glGetUniformLocation(shadersId, "Texture"), 1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // render target
    normMatrixLoc = glGetUniformLocation(shadersId, "normMatrix");
    glUniformMatrix4fv(normMatrixLoc, 1, GL_FALSE, &normMatrix[0][0]);
    glUniform1i(glGetUniformLocation(shadersId, "target"), 1);

    glUniform1i(glGetUniformLocation(shadersId, "Texture"), 0);

    glDrawArrays(GL_POLYGON, 3, targetPointsCount);

    destroyVBO();

    glutSwapBuffers();
    glutPostRedisplay();
    glFlush();
}

void Scene::run(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(initialWindowsPosX, initialWindowsPosY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Target follower");

	glewInit();
	setup();

	glutDisplayFunc(renderWrapper);
	glutCloseFunc(cleanupWrapper);

	glutMainLoop();
}

void renderWrapper() {

	Scene::getInstance().render();
}

void cleanupWrapper() {

	Scene::getInstance().cleanup();
}