#ifndef OBJWriter_H
#define OBJWriter_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

using namespace std;

class OBJWriter
{
public:
	ofstream OBJFile;

	OBJWriter();
	void createOBJFile();

	void addFaces(int index, int externalCurveSize, int faces, int n);

	void addNormalExternalCurve(glm::vec3 normal_vec_abac, glm::vec3 normal_vec_dbdc);

	void addPointsFinalCurve(float x, float y, float z);

	void saveTextureValuesToOBJ();
};

#endif