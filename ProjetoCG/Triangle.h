#pragma once

#include <glm.hpp>
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include "globals.h"

class Triangle {

public:

	Triangle();

	Triangle(glm::vec3 position1, glm::vec3 position2, glm::vec3 _position3, glm::vec4 color);

	void draw();

	// translation é um vec3 que define quanto vamos transladar nos eixos x, y e z
	void translate(glm::vec3 translation);

	// axis define o eixo de rotação
	void rotate(GLfloat angle, glm::vec3 axis, glm::vec3 center);

	// scale é um vec3 que define quanto vamos escalar nos eixos x, y e z
	void scale(glm::vec3 scale);

	void printarPontos();

	glm::vec3 getPosition1() const;
	glm::vec3 getPosition2() const;

	glm::vec4 getColor() const;
	void setColor(glm::vec4 color);

	glm::vec3 getVectorTranslation() const;
	glm::vec3 getVectorRotation() const;
	glm::vec3 getVectorScale() const;

	glm::mat4 getMatrixTranslation() const;
	glm::mat4 getMatrixRotation() const;
	glm::mat4 getMatrixScale() const;
	glm::mat4 getMatrixModel() const;

	glm::vec3 _position1;
	glm::vec3 _position2;
	glm::vec3 _position3;

private:

	glm::vec4 _color;

	glm::vec3 _vectorTranslation;
	glm::vec3 _vectorRotation;
	glm::vec3 _vectorScale;

	unsigned int _VAO;
	unsigned int _VBO;

	glm::mat4 _matrixTranslation;
	glm::mat4 _matrixRotation;
	glm::mat4 _matrixScale;
	glm::mat4 _matrixModel;

	void updateModel();

	void CreateBuffers();
	void FillBuffers();
	void LinkBuffers();

};

