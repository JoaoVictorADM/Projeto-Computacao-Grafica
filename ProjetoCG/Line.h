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

		glm::vec3 getVectorTranslation() const;
		glm::vec3 getVectorRotation() const;
		glm::vec3 getVectorScale() const;

		glm::mat4 getMatrixTranslation() const;
		glm::mat4 getMatrixRotation() const;
		glm::mat4 getMatrixScale() const;
		glm::mat4 getMatrixModel() const;

	private: 

		glm::vec3 _position1;
		glm::vec3 _position2;
		glm::vec3 _color1;
		glm::vec3 _color2;

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

