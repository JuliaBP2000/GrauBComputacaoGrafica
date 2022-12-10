#include "Object.h"


void Object::startCurve() {

	createCurve();

	finishedCurve = getPointsFloat();
	 //finishcurve é o vertex

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, finishedCurve->size() * sizeof(GLfloat), &finishedCurve->at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);



}

void Object::createCurve() {

	//start B-Spline
	std::vector<glm::vec3*>* aux = new std::vector<glm::vec3*>();

	for (int i = 0; i < initialPoints->size(); i++) {
		aux->push_back(new glm::vec3(initialPoints->at(i)->x, initialPoints->at(i)->y, 0));
	}

	aux->push_back(initialPoints->at(0));
	aux->push_back(initialPoints->at(1));
	aux->push_back(initialPoints->at(2));

	//Calculo da B-Spline
	for (int i = 0; i < (aux->size() - 3); i++) {
		for (int j = 0; j < 100; ++j) {

			float t = static_cast<float>(j) / 99.0;

			GLfloat x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * aux->at(i)->x +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * aux->at(i + 1)->x +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * aux->at(i + 2)->x +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * aux->at(i + 3)->x) / 6);

			GLfloat y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * aux->at(i)->y +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * aux->at(i + 1)->y +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * aux->at(i + 2)->y +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * aux->at(i + 3)->y) / 6);

			glm::vec3* point = new glm::vec3(x, y, 0.0);

			//adiciona os potos na curva
			curvePoints->push_back(point);

			curvePoints->push_back(new glm::vec3(0.0, 0.1, 1.0));

		}
	}

	//gera curvas interna e externa
	outCurve = internExternCurves(HALF_PI);//externa
	inCurve = internExternCurves(-HALF_PI);//interna

	finishCurve();

}

std::vector<glm::vec3*>* Object::internExternCurves(float toSum) {

	OBJWriter OBJWriter;
	std::vector<glm::vec3*>* calculatedCurve = new std::vector<glm::vec3*>();

	for (int j = 0; j < curvePoints->size() - 1; j += 2) {

		glm::vec3* a = curvePoints->at(j);
		glm::vec3* b;

		if (j == curvePoints->size() - 2) {
			b = curvePoints->at(0);//pega valor inicio
		}
		else {
			b = curvePoints->at(j + 2);//pega 2 posiçoes a frente do atual
		}

		GLfloat dx = b->x - a->x;
		GLfloat dy = b->y - a->y;

		if (dx == 0 || dy == 0) {
			dx = b->x - curvePoints->at(j - 2)->x;
			dy = b->y - curvePoints->at(j - 2)->y;
		}

		GLfloat angle = glm::atan(dy, dx);
		angle += toSum;

		GLfloat offsetX = glm::cos(angle) * 0.09;
		GLfloat offsetY = glm::sin(angle) * 0.09;

		glm::vec3* generatePoints = new glm::vec3(a->x + offsetX, a->y + offsetY, 0.0);

		calculatedCurve->push_back(generatePoints);

		OBJWriter.addPointsFinalCurve(generatePoints->x, generatePoints->y, generatePoints->z);

		calculatedCurve->push_back(new glm::vec3(0.0, 0.1, 1.0));

	}
	return calculatedCurve;
}

void Object::finishCurve() {
	OBJWriter OBJWriter;
	OBJWriter.saveTextureValuesToOBJ();

	int extSize = outCurve->size() / 2.0;
	int faces = 0;

	int i = 0;
	int index = 1;

	for (; i < inCurve->size() - 2; i += 2) {

		resultPoints->push_back(inCurve->at(i));
		resultPoints->push_back(inCurve->at(i + 1));

		glm::vec3* a_in = inCurve->at(i);

		resultPoints->push_back(inCurve->at(i + 2));
		resultPoints->push_back(inCurve->at(i + 3));

		glm::vec3* b_in = inCurve->at(i + 2);

		resultPoints->push_back(outCurve->at(i));
		resultPoints->push_back(outCurve->at(i + 1));

		glm::vec3* c_ext = outCurve->at(i);

		OBJWriter.addFaces(index, extSize, ++faces, 1);

		resultPoints->push_back(inCurve->at(i + 2));
		resultPoints->push_back(inCurve->at(i + 3));

		resultPoints->push_back(outCurve->at(i + 2));
		resultPoints->push_back(outCurve->at(i + 3));

		glm::vec3* d_ext = outCurve->at(i + 2);

		resultPoints->push_back(outCurve->at(i));
		resultPoints->push_back(outCurve->at(i + 1));

		OBJWriter.addFaces(index, extSize, ++faces, 2);

		glm::vec3 ab = glm::vec3(b_in->x - a_in->x, b_in->z - a_in->z, b_in->y - a_in->y);
		glm::vec3 ac = glm::vec3(c_ext->x - a_in->x, c_ext->z - a_in->z, c_ext->y - a_in->y);
		glm::vec3 dc = glm::vec3(c_ext->x - d_ext->x, c_ext->z - d_ext->z, c_ext->y - d_ext->y);
		glm::vec3 db = glm::vec3(b_in->x - d_ext->x, b_in->z - d_ext->z, b_in->y - d_ext->y);

		glm::vec3 normal_vec_abac = glm::cross(ac, ab);
		glm::vec3 normal_vec_dbdc = glm::cross(db, dc);

		OBJWriter.addNormalExternalCurve(normal_vec_abac, normal_vec_dbdc);

		index++;
	}

	resultPoints->push_back(inCurve->at(i));
	resultPoints->push_back(inCurve->at(i + 1));

	glm::vec3* a_int = inCurve->at(i);

	resultPoints->push_back(inCurve->at(0));
	resultPoints->push_back(inCurve->at(1));

	glm::vec3* b_int = inCurve->at(0);

	resultPoints->push_back(outCurve->at(i));
	resultPoints->push_back(outCurve->at(i + 1));

	glm::vec3* c_ext = outCurve->at(i);

	OBJWriter.addFaces(index, extSize, ++faces, 3);

	resultPoints->push_back(inCurve->at(0));
	resultPoints->push_back(inCurve->at(1));

	resultPoints->push_back(outCurve->at(0));
	resultPoints->push_back(outCurve->at(1));

	glm::vec3* d_ext = outCurve->at(0);

	resultPoints->push_back(outCurve->at(i));
	resultPoints->push_back(outCurve->at(i + 1));

	OBJWriter.addFaces(index, extSize, ++faces, 4);

	glm::vec3 ab = glm::vec3(a_int->x - b_int->x, a_int->z - b_int->z, a_int->y - b_int->y);
	glm::vec3 ac = glm::vec3(a_int->x - c_ext->x, a_int->z - c_ext->z, a_int->y - c_ext->y);
	glm::vec3 dc = glm::vec3(d_ext->x - c_ext->x, d_ext->z - c_ext->z, d_ext->y - c_ext->y);
	glm::vec3 db = glm::vec3(d_ext->x - b_int->x, d_ext->z - b_int->z, d_ext->y - b_int->y);

	glm::vec3 normal_vec_abac = glm::cross(ab, ac);
	glm::vec3 normal_vec_dbdc = glm::cross(db, dc);

	OBJWriter.addNormalExternalCurve(normal_vec_abac, normal_vec_dbdc);

}

std::vector<GLfloat>* Object::getPointsFloat() {

	std::vector<GLfloat>* result = new std::vector<GLfloat>();

	for (int i = 0; i < resultPoints->size(); i++) {
		result->push_back(resultPoints->at(i)->x);
		result->push_back(resultPoints->at(i)->y);
		result->push_back(resultPoints->at(i)->z);
	}
	return result;
}

void Object::addNewPoint(double x, double y)
{

	double width = WIDTH / 2;
	double height = HEIGHT / 2;

	//transforma em cordenadas de -1 a 1

	if (x > width) {
		x = ((x - width) / width);
	}
	else if (x == width) {
		x = 0;
	}
	else {
		x = -((width - x) / width);
	}

	if (y > height) {
		y = ((y - height) / height) * (-1);
	}
	else if (y == height) {
		y = 0;
	}
	else {
		y = -((height - y) / height) * (-1);
	}

	glm::vec3* point = new glm::vec3(x, y, 0.0);

	//printf("Pontos apos divisão: [%f , %f]", x, y);

	initialPoints->push_back(point);

	//coordenadas em memorias
	//limpa valores
	if (x > 0.0 && y > 0.0) {
		x += 0.5;
		y += 0.5;
	}
	else if (x > 0.0 && y < 0.0) {
		x += 0.5;
		y -= 0.5;
	}
	else if (x < 0.0 && y < 0.0) {
		x -= 0.5;
		y -= 0.5;
	}
	else {
		x -= 0.5;
		y += 0.5;
	}
}

void Object::draw(Shader shader)
{
	shader.Use();
	glDrawArrays(GL_TRIANGLES, 0, finishedCurve->size());
}

void Object::setShader(Shader* shader)
{
	Object::shader = shader;
}