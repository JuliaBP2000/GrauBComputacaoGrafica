#pragma once
// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <vector>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"

#include "OBJWriter.h"

#define PI 3.14159265359
#define HALF_PI PI/2.0

const GLint WIDTH = 600, HEIGHT = 600;
class Object
{
public:
	void startCurve();

	void createCurve();

	std::vector<glm::vec3*>* internExternCurves(float toSum);

	void finishCurve();

	std::vector<GLfloat>* getPointsFloat();

	void addNewPoint(double x, double y);

	void draw(Shader shader);

	void setShader(Shader* shader);
private:
	std::vector<glm::vec3*>* resultPoints = new std::vector<glm::vec3*>();
	std::vector<glm::vec3*>* curvePoints = new std::vector<glm::vec3*>();
	std::vector<glm::vec3*>* outCurve = new std::vector<glm::vec3*>();
	std::vector<glm::vec3*>* inCurve = new std::vector<glm::vec3*>();


	std::vector<GLfloat>* finishedCurve = new std::vector<GLfloat>();
	std::vector<glm::vec3*>* initialPoints = new std::vector<glm::vec3*>();

	Shader* shader;

	GLuint VBO, VAO;
};

