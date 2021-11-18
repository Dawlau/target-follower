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

void Scene::loadTexture() {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height;
    unsigned char* image = SOIL_load_image("text_smiley_face.png", &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Scene::cleanup() {

	destroyVBO();
	unloadShaders();
}

void Scene::setup() {

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // white background

    std::vector < glm::vec2 > targetPoints = target.getPoints();

    for(int i = 0; i < targetPoints.size(); ++i) {

        int idx = (i + followerPointsCount) * vertexSize; // add 3 because the first 3 points in the vbo are follower points

        vertices[idx] = targetPoints[i][0];
        vertices[idx + 1] = targetPoints[i][1]; // coordinates
        vertices[idx + 2] = 0.0f;
        vertices[idx + 3] = 1.0f;

        vertices[idx + 4] = 0.0f;
        vertices[idx + 5] = 0.0f; // colors
        vertices[idx + 6] = 0.0f;

        vertices[idx + 7] = targetPoints[i][0] / windowWidth; // texture coordinates
        vertices[idx + 8] = targetPoints[i][1] / windowHeight;
    }

    createVBO();
    loadTexture();
    loadShaders();


}

void Scene::render() {

	glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(!follower.getLiveAnimation()){
        follower.updateAnimation(target);
    }

    glm::mat4 followerTransformation = follower.getTransformation();
    followerTransformation = normMatrix * followerTransformation;

    // glm::vec4 o = rotMat * glm::vec4(100.0f, 100.0f, 0.0f, 1.0f);

    // for(int i = 0; i < 4; ++i){
    //     std::cout << std::fixed << std::setprecision(6) << o[i] << '\n';
    // }

    GLuint normMatrixLoc = glGetUniformLocation(shadersId, "normMatrix");
    glUniformMatrix4fv(normMatrixLoc, 1, GL_FALSE, &followerTransformation[0][0]);

    glUniform1i(glGetUniformLocation(shadersId, "Texture"), 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    normMatrixLoc = glGetUniformLocation(shadersId, "normMatrix");
    glUniformMatrix4fv(normMatrixLoc, 1, GL_FALSE, &normMatrix[0][0]);

    // glPointSize(20.0f);
    glDrawArrays(GL_POLYGON, 3, targetPointsCount);

    // glPopMatrix();
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