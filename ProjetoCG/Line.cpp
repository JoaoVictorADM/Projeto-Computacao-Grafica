#include "Line.h"

Line::Line(){}

Line::Line(glm::vec3 position1, glm::vec3 position2, glm::vec4 color) :
	_vectorTranslation(glm::vec3(0.0f)),
	_vectorRotation(glm::vec3(0.0f)),
	_vectorScale(glm::vec3(1.0f)),
	_matrixTranslation(glm::mat4(1.0f)),
	_matrixRotation(glm::mat4(1.0f)),
	_matrixScale(glm::mat4(1.0f)),
	_matrixModel(glm::mat4(1.0f)),
	_VAO(0),
	_VBO(0)
{
	_position1 = position1;
	_position2 = position2;
	_color = color;

	CreateBuffers();
	FillBuffers();
	LinkBuffers();
}

void Line::draw(){

	glBindVertexArray(_VAO);
	
	{
		objectShader.SendUniformData("colorOut", _color);
		objectShader.SendUniformData("Matrix", ortho * _matrixModel);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glBindVertexArray(0);

}

void Line::translate(glm::vec3 translation){
	_vectorTranslation += translation;
	_matrixTranslation = glm::translate(glm::mat4(1.0f), _vectorTranslation);
	updateModel();
}

void Line::rotate(GLfloat angle, glm::vec3 axis, glm::vec3 center){

	_vectorRotation += axis * angle;

	glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -center); // Translação para o ponto médio

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(_vectorRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(_vectorRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(_vectorRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), center); // Translação de volta para a posição original

	_matrixRotation = translationBack * rotation * translationToOrigin;

	updateModel();

}

void Line::scale(glm::vec3 scale){
	_vectorScale += scale;
	_matrixScale = glm::scale(glm::mat4(1.0f), _vectorScale);
	updateModel();
}

void Line::printarPontos(){

	printf("Ponto 1: (%f, %f, %f)\n", _position1.x, _position1.y, _position1.z);
	printf("Ponto 2: (%f, %f, %f)\n\n", _position2.x, _position2.y, _position2.z);

}

glm::vec3 Line::getPosition1() const{

	glm::vec4 position1 = glm::vec4(_position1, 1.0f);

	return glm::vec3(_matrixModel * position1);
	
}

glm::vec3 Line::getPosition2() const{

	glm::vec4 position2 = glm::vec4(_position2, 1.0f);

	return glm::vec3(_matrixModel * position2);

}

glm::vec4 Line::getColor() const{
	return _color;
}

void Line::setColor(glm::vec4 color){
	_color = color;
}

glm::vec3 Line::getVectorTranslation() const{
	return _vectorTranslation;
}

glm::vec3 Line::getVectorRotation() const{
	return _vectorRotation;
}

glm::vec3 Line::getVectorScale() const{
	return _vectorScale;
}

glm::mat4 Line::getMatrixTranslation() const{
	return _matrixTranslation;
}

glm::mat4 Line::getMatrixRotation() const{
	return _matrixRotation;
}

glm::mat4 Line::getMatrixScale() const{
	return _matrixScale;
}

glm::mat4 Line::getMatrixModel() const{
	return _matrixModel;
}

void Line::updateModel(){
	_matrixModel = _matrixTranslation * _matrixRotation * _matrixScale;
}

void Line::CreateBuffers(){
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
}	

void Line::FillBuffers(){

	GLfloat vertices[] = {
		_position1.x, _position1.y, _position1.z,
		_position2.x, _position2.y, _position2.z
	};

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Line::LinkBuffers(){

	glBindVertexArray(_VAO);

	{
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
		
	}

	glBindVertexArray(_VAO);
}