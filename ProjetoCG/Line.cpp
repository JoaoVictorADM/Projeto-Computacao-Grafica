#include "Line.h"

#include<GL/glew.h>
#include<glm/gtc/matrix_transform.hpp>

Line::Line(glm::vec3 position1, glm::vec3 position2, glm::vec3 color1, glm::vec3 color2) :
	_translation(glm::mat4(1.0f)),
	_rotation(glm::mat4(1.0f)),
	_scale(glm::mat4(1.0f)),
	_model(glm::mat4(1.0f)), 
	_VAO(0),
	_VBO(0)
{
	_position1 = position1;
	_position2 = position2;
	_color1 = color1;
	_color2 = color2;

	CreateBuffers();
	FillBuffers();
	LinkBuffers();
}

void Line::draw(){

	glBindVertexArray(_VAO);
	
	{
		glDrawArrays(GL_LINES, 0, 2);
	}

	glBindVertexArray(0);

}

void Line::translate(glm::vec3 translation){
	_translation = glm::translate(glm::mat4(1.0f), translation);
	updateModel();
}

void Line::rotate(GLfloat angle, glm::vec3 axis, glm::vec3 center){

	glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -center); // Translação para o ponto médio
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis); // Rotação
	glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), center); // Translação de volta para a posição original

	_rotation = translationBack * rotation * translationToOrigin;

	updateModel();

}

void Line::scale(glm::vec3 scale) {
	_scale = glm::scale(glm::mat4(1.0f), scale);
	updateModel();
}

void Line::printarPontos(){

	printf("Ponto 1: (%f, %f, %f)\n", _position1.x, _position1.y, _position1.z);
	printf("Ponto 2: (%f, %f, %f)\n\n", _position2.x, _position2.y, _position2.z);

}

glm::mat4 Line::getTranslation() const{
	return _translation;
}

glm::mat4 Line::getRotation() const{
	return _rotation;
}

glm::mat4 Line::getScale() const{
	return _scale;
}

glm::mat4 Line::getModel() const{
	return _model;
}

void Line::updateModel(){
	_model = _translation * _rotation * _scale;
}

void Line::CreateBuffers(){
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
}

void Line::FillBuffers(){

	GLfloat vertices[] = {
		_position1.x, _position1.y, _position1.z, _color1.x, _color1.y, _color1.z,
		_position2.x, _position2.y, _position2.z, _color2.x, _color2.y, _color2.z
	};

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Line::LinkBuffers(){

	glBindVertexArray(_VAO);

	{
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	}

	glBindVertexArray(_VAO);
}