#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include<string>
#include<fstream>
#include<sstream>

#include "Error.h"
#include "Shader.h"
#include "Line.h"

void initOpenGL();
std::vector<Line> createStar();
glm::vec3 calculateCenter(GLfloat* vertexVector);

GLFWwindow *window = NULL;

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char *gWindowTitle = "Projeto Computa��o Gr�fica";

GLfloat left = -80.0f;
GLfloat right = 80.0f;
GLfloat bottom = -80.0f; 
GLfloat top = 80.0f;
GLfloat near = -1.0f;
GLfloat far = 1.0f;

glm::mat4 ortho = glm::ortho(left, right, bottom, top, near, far);

GLfloat starVertexGlobal[] = {
    // X       Y       Z     R     G     B 
    21.18f,    5.0f,  0.0f, 1.0f, 0.0f, 0.0f, // Ponto F 
    13.09f,  -19.9f,  0.0f, 0.0f, 1.0f, 0.0f, // Ponto G 
   -13.09f,  -19.9f,  0.0f, 0.0f, 0.0f, 1.0f, // Ponto H 
   -21.18f,    5.0f,  0.0f, 1.0f, 1.0f, 0.0f, // Ponto I 
     0.0f,   20.39f,  0.0f, 0.0f, 1.0f, 1.0f, // Ponto J 
    -5.0f,     5.0f,  0.0f, 0.5f, 0.5f, 0.0f, // Ponto F (Interse��o) 
     5.0f,     5.0f,  0.0f, 0.5f, 0.0f, 0.5f, // Ponto G (Interse��o) 
     8.09f,  -4.51f,  0.0f, 0.0f, 0.5f, 0.5f, // Ponto H (Interse��o) 
     0.0f,  -10.39f,  0.0f, 0.5f, 0.5f, 0.5f, // Ponto I (Interse��o) 
    -8.09f,  -4.51f,  0.0f, 1.0f, 0.5f, 0.0f  // Ponto J (Interse��o) 
};

int main(){

    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    shader.Bind();

    GLint pulos = 8;
    GLfloat posY = 0.0f;
    GLfloat speedY = 0.01f;
	GLfloat angle = 0.0f;
    GLfloat passoAngle = 0.01f;
	GLfloat posX = 0.0f;
    GLfloat passoX = 0.005f;

    std::vector<Line> starLines = createStar();

    glm::vec3 center = calculateCenter(starVertexGlobal);

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       

        if(pulos > 0){

            posY += speedY;

            for (int i = 0; i < 15; i++) {

                starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));

            }

            if (posY <= 0.0f || posY >= 20.0f) {

                speedY *= -1.0f;

                if (posY <= 0.0f) {

                    pulos--;

                }

            }
    
        }

        if(angle <= 90.0f && pulos == 0){
			angle += passoAngle;
            posX += passoX;

            for(int i = 0; i < 15; i++){

                starLines[i].rotate(-passoAngle, glm::vec3(0.0f, 0.0f, 1.0f), center);
                starLines[i].translate(glm::vec3(passoX, 0.0f, 0.0f));

            }
        }

        for(int i = 0; i < 15; i++) {

            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
            
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void initOpenGL(){

    if (!glfwInit())
        fatalError("GLFW could not be initialized");

    window = glfwCreateWindow(gWindowWidth, gWindowHeight, gWindowTitle, NULL, NULL);

    if (!window)
        fatalError("GLFW Window could not be created!");

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (GLEW_OK != err)
        fatalError("Error loading GLEW extensions!");

    glEnable(GL_DEPTH_TEST);

}

std::vector<Line> createStar(){

    GLuint indexVertexStar[] = {
        6, 0,
        5, 6,
        3, 5,
        6, 7,
        7, 1,
        2, 8,
        8, 9,
        2, 9,
        9, 3,
        9, 5,
        5, 4,
        4, 6,
        0, 7,
        7, 8,
        1, 8
    };

    std::vector<Line> starLines;

    for(int i = 0; i < 15; i++){

        GLuint indiceLinha1 = indexVertexStar[i * 2] * 6;
        GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 6;

        starLines.emplace_back(
            glm::vec3(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]),
            glm::vec3(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]),
            glm::vec3(starVertexGlobal[indiceLinha1 + 3], starVertexGlobal[indiceLinha1 + 4], starVertexGlobal[indiceLinha1 + 5]),
            glm::vec3(starVertexGlobal[indiceLinha2 + 3], starVertexGlobal[indiceLinha2 + 4], starVertexGlobal[indiceLinha2 + 5])
        );

    }

    return starLines;
}

glm::vec3 calculateCenter(GLfloat *vertexVector) {

    glm::vec3 center(0.0f, 0.0f, 0.0f);
    size_t numVertices = 10;

    for (size_t i = 0; i < numVertices; ++i) {
        center += glm::vec3(vertexVector[i * 6], vertexVector[i * 6 + 1], vertexVector[i * 6 + 2]);
    }

    center /= static_cast<float>(numVertices);

    return center;
}
