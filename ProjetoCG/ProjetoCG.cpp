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

const float EPSILON = 0.001f;

GLFWwindow *window = NULL;

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char *gWindowTitle = "Projeto Computação Gráfica";

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
    -5.0f,     5.0f,  0.0f, 0.5f, 0.5f, 0.0f, // Ponto F (Interseção) 
     5.0f,     5.0f,  0.0f, 0.5f, 0.0f, 0.5f, // Ponto G (Interseção) 
     8.09f,  -4.51f,  0.0f, 0.0f, 0.5f, 0.5f, // Ponto H (Interseção) 
     0.0f,  -10.39f,  0.0f, 0.5f, 0.5f, 0.5f, // Ponto I (Interseção) 
    -8.09f,  -4.51f,  0.0f, 1.0f, 0.5f, 0.0f  // Ponto J (Interseção) 
};

int main(){

    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    shader.Bind();

    GLboolean jumpScene = true;
    GLboolean spinRightScene = false;
    GLboolean dismantleScene = false;

    GLint jumps = 1;
    GLfloat speedY = 0.01f;
    GLfloat speedAngle = 0.01f;
    GLfloat speedX = 0.005f;

    std::vector<Line> starLines = createStar();

    glm::vec3 center = calculateCenter(starVertexGlobal);

    while(!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if(jumpScene){

            for(int i = 0; i < 15; i++) {

                starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
                shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
                starLines[i].draw();

            }

            if(starLines[0].getVectorTranslation().y <= 0.0f || starLines[0].getVectorTranslation().y >= 20.0f) {

                speedY *= -1.0f;

                if(starLines[0].getVectorTranslation().y <= 0.0f){

                    jumps--;

                    if(jumps == 0){
                        jumpScene = false;
                        spinRightScene = true;
                        
                    }

                }

            }

        }

        if(spinRightScene){

            for (int i = 0; i < 15; i++) {

                starLines[i].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), center);
                starLines[i].translate(glm::vec3(speedX, 0.0f, 0.0f));

                shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
                starLines[i].draw();

            }

            if(fabs(starLines[0].getVectorRotation().z + 90.0f) < EPSILON){
                spinRightScene = false;
                dismantleScene = true;
            }
        }

        if(dismantleScene){

            for (int i = 0; i < 15; i++) {

                if (((i == 3 || i == 4) && starLines[14].getVectorRotation().z < -17.90f) ||
                    (i == 5 && starLines[14].getVectorRotation().z < 54.0f) ||
                    (i == 6 && starLines[14].getVectorRotation().z < 126.0f) ||
                    (i >= 7)
                    ){

                    starLines[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), center);
                    starLines[i].translate(glm::vec3(-speedX, 0.0f, 0.0f));

                }

				if(i == 6 && starLines[14].getVectorRotation().z >= 180.0f && starLines[6].getVectorTranslation().y ) {
                    starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
				}

                shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
                starLines[i].draw();
            }

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
