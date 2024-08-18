#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class Line{

	public:

		Line(glm::vec3 position1, glm::vec3 position2, glm::vec3 color1, glm::vec3 color2);

		void draw();

		// translation é um vec3 que define quanto vamos transladar nos eixos x, y e z
		void translate(glm::vec3 translation);

		// axis define o eixo de rotação
		void rotate(GLfloat angle, glm::vec3 axis, glm::vec3 center);

		// scale é um vec3 que define quanto vamos escalar nos eixos x, y e z
		void scale(glm::vec3 scale);

		void printarPontos();

		glm::mat4 getTranslation() const;
		glm::mat4 getRotation() const;
		glm::mat4 getScale() const;
		glm::mat4 getModel() const;

	private: 

		glm::vec3 _position1;
		glm::vec3 _position2;
		glm::vec3 _color1;
		glm::vec3 _color2;

		unsigned int _VAO;
		unsigned int _VBO;

		glm::mat4 _translation;
		glm::mat4 _rotation;
		glm::mat4 _scale;
		glm::mat4 _model;
		
		void updateModel();

		void CreateBuffers();
		void FillBuffers();
		void LinkBuffers();

};

