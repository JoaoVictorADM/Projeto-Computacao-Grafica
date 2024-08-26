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

/* Functions */

void initOpenGL();
std::vector<Line> createStar();
glm::vec3 calculateCenter(GLfloat* vertexVector);

void jumpScene();
void spinRightScene();
void dismantleScene();
void goldenRuleScene();
void goldenRuleSceneStep1(Line& line);
void goldenRuleSceneStep2(Line& line);
void goldenRuleSceneStep3(Line& line, GLfloat positionXLinha2);
void goldenRuleSceneStep4(Line& line);
void goldenRuleSceneStep5(Line& line);
void goldenRuleSceneStep6(Line& line);
void goldenRuleSceneStep7(Line& line, GLfloat positionXLinha3);
void goldenRuleSceneStep8(Line& line);
void reassembleScene();

/* Infos Window */

GLFWwindow* window = NULL;

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char* gWindowTitle = "Projeto Computação Gráfica";

GLfloat left = -80.0f;
GLfloat right = 80.0f;
GLfloat bottom = -80.0f;
GLfloat top = 80.0f;
GLfloat near = -1.0f;
GLfloat far = 1.0f;

glm::mat4 ortho = glm::ortho(left, right, bottom, top, near, far);

/* for float comparison */

const float EPSILON = 0.01f;

GLfloat starVertexGlobal[] = {
    // X       Y       Z     R     G     B 
    21.18f,    5.0f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto F 
    13.09f,  -19.9f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto G 
   -13.09f,  -19.9f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto H 
   -21.18f,    5.0f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto I 
     0.0f,   20.39f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto J 
    -5.0f,     5.0f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto F (Interseção) 
     5.0f,     5.0f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto G (Interseção) 
     8.09f,  -4.51f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto H (Interseção) 
     0.0f,  -10.39f,  0.0f, 1.0f, 0.0f, 0.5f, // Ponto I (Interseção) 
    -8.09f,  -4.51f,  0.0f, 1.0f, 0.0f, 0.5f  // Ponto J (Interseção) 
};

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

Shader shader;

std::vector<Line> starLines;
glm::vec3 centerStar;

int main(){

    initOpenGL();

    shader = Shader("Main.vert", "Main.frag");

    shader.CreateShaders();
    shader.Bind();

    starLines = createStar();
    centerStar = calculateCenter(starVertexGlobal);

    jumpScene();
    spinRightScene();
    dismantleScene();
    goldenRuleScene();
    reassembleScene();
    jumpScene();

    glfwTerminate();
    return 0;

}

void initOpenGL(){

    if(!glfwInit())
        fatalError("GLFW could not be initialized");

    window = glfwCreateWindow(gWindowWidth, gWindowHeight, gWindowTitle, NULL, NULL);

    if(!window)
        fatalError("GLFW Window could not be created!");

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if(GLEW_OK != err)
        fatalError("Error loading GLEW extensions!");

    glEnable(GL_DEPTH_TEST);

}

std::vector<Line> createStar(){

    std::vector<Line> starLines;

    for(int i = 0; i < 15; i++){

        GLuint indiceLinha1 = indexVertexStar[i * 2] * 6;
        GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 6;

        starLines.emplace_back(
            glm::vec3(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]),
            glm::vec3(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]),
            glm::vec4(starVertexGlobal[indiceLinha1 + 3], starVertexGlobal[indiceLinha1 + 4], starVertexGlobal[indiceLinha1 + 5], 1.0f),
            glm::vec4(starVertexGlobal[indiceLinha2 + 3], starVertexGlobal[indiceLinha2 + 4], starVertexGlobal[indiceLinha2 + 5], 1.0f)
        );

    }

    return starLines;
}

glm::vec3 calculateCenter(GLfloat* vertexVector){

    glm::vec3 center(0.0f, 0.0f, 0.0f);
    size_t numVertices = 10;

    for(size_t i = 0; i < numVertices; ++i){
        center += glm::vec3(vertexVector[i * 6], vertexVector[i * 6 + 1], vertexVector[i * 6 + 2]);
    }

    center /= static_cast<float>(numVertices);

    return center;
}

void jumpScene(){

    GLuint jumps = 1;

	GLfloat speedY = 0.01f;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < 15; i++){
            starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();

        }

        if(starLines[0].getVectorTranslation().y <= 0.0f || starLines[0].getVectorTranslation().y >= 20.0f){

            speedY *= -1.0f;

            if(starLines[0].getVectorTranslation().y <= 0.0f){

                jumps--;

                if(jumps == 0)
                    break;
                

            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void spinRightScene(){

    GLfloat speedX = 0.005f;
    GLfloat speedAngle = 0.01f;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < 15; i++){

            starLines[i].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
            starLines[i].translate(glm::vec3(speedX, 0.0f, 0.0f));

            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();

        }

        if(fabs(starLines[0].getVectorRotation().z + 90.0f) < EPSILON)
            break;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void dismantleScene(){

    GLfloat speedX = 0.005f;
    GLfloat speedY = 0.01f;
    GLfloat speedAngle = 0.01f;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(dismantleScene){

            for(int i = 0; i < 15; i++){

                if (((i == 3 || i == 4) && starLines[14].getVectorRotation().z < -17.90f) ||
                    (i == 5 && starLines[14].getVectorRotation().z < 54.0f) ||
                    (i == 6 && starLines[14].getVectorRotation().z < 126.0f) ||
                    (i >= 7)
                    ) {

                    starLines[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
                    starLines[i].translate(glm::vec3(-speedX, 0.0f, 0.0f));

                }

                if(i == 6 && starLines[14].getVectorRotation().z >= 180.0f && starLines[6].getPosition2().y >= starLines[0].getPosition2().y)
                    starLines[i].translate(glm::vec3(0.0f, -speedY, 0.0f));
                

                if(starLines[14].getVectorRotation().z >= 220.0f)
                    return;
                

                shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
                starLines[i].draw();
            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleScene(){

    GLfloat positionXLinha2 = starLines[5].getPosition2().x;
    GLfloat positionXLinha3 = starLines[4].getPosition2().x;

    Line line1 = Line(starLines[6].getPosition1(),
                 starLines[6].getPosition2(),
                 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    Line line2 = Line(starLines[5].getPosition1(),
                 starLines[5].getPosition2(),
                 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    goldenRuleSceneStep1(line1);
    goldenRuleSceneStep2(line1);
    goldenRuleSceneStep3(line1, positionXLinha2);
    goldenRuleSceneStep4(line1);
    goldenRuleSceneStep5(line2);
    goldenRuleSceneStep6(line2);
    goldenRuleSceneStep7(line2, positionXLinha3);
    goldenRuleSceneStep8(line2);


}

void goldenRuleSceneStep1(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[6].getPosition2().x)) / 1500.0f;
        GLfloat speedY = (starLines[5].getPosition2().y - starLines[6].getPosition2().y) / 1500.0f;

        line.translate(glm::vec3(speedX, speedY, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        if(fabs(line.getPosition2().x - starLines[5].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep2(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[4].getPosition2().x)) / 1500.0f;

        line.translate(glm::vec3(speedX, 0.0f, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[5].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON + 0.5f){

            for (float i = 0.0f; i < 1000.0f; i += 0.001f) {
                printf("");
            }

            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep3(Line& line, GLfloat positionXLinha2){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[5].getPosition2().x - positionXLinha2)) / 1500.0f;

        line.translate(glm::vec3(-speedX, 0.0f, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[5].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha2) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 100.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep4(Line& line){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void goldenRuleSceneStep5(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[4].getPosition2().x)) / 2500.0f;
        GLfloat speedY = (starLines[3].getPosition1().y - starLines[4].getPosition2().y) / 2500.0f;

        line.translate(glm::vec3(speedX, speedY, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep6(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[4].getPosition2().x - starLines[0].getPosition1().x)) / 1500.0f;

        line.translate(glm::vec3(speedX, 0.0f, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[3].translate(glm::vec3(speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[0].getPosition1().x) < EPSILON + 0.5f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
                
            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep7(Line& line, GLfloat positionXLinha3){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedX = fabs((starLines[4].getPosition2().x - positionXLinha3)) / 1500.0f;

        line.translate(glm::vec3(-speedX, 0.0f, 0.0f));

        shader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[3].translate(glm::vec3(-speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha3) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep8(Line& line){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void reassembleScene(){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat speedReassemble = 0.01f;

        // Montar as linhas restantes do pentagrama
        for(int i = 0; i < 15; i++){

            GLuint indiceLinha1 = indexVertexStar[i * 2] * 6;
            GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 6;

            glm::vec3 targetPos1(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]);
            glm::vec3 targetPos2(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]);

            glm::vec3 currentPos1 = starLines[i].getPosition1();
            glm::vec3 currentPos2 = starLines[i].getPosition2();

            glm::vec3 direction1 = glm::normalize(targetPos1 - currentPos1);
            glm::vec3 direction2 = glm::normalize(targetPos2 - currentPos2);

            glm::vec3 newPos1 = currentPos1 + direction1 * speedReassemble;
            glm::vec3 newPos2 = currentPos2 + direction2 * speedReassemble;

            // Atualizar a posição da linha
            starLines[i] = Line(newPos1, newPos2,
                glm::vec4(1.0f, 0.0f, 0.5f, 1.0f),
                glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));

            shader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        // Verificar se a montagem está completa
        bool montageComplete = true;

        for(int i = 7; i < 15; i++){
            GLuint indiceLinha1 = indexVertexStar[i * 2] * 6;
            GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 6;

            glm::vec3 targetPos1(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]);
            glm::vec3 targetPos2(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]);

            if (glm::distance(starLines[i].getPosition1(), targetPos1) > EPSILON ||
                glm::distance(starLines[i].getPosition2(), targetPos2) > EPSILON) {
                montageComplete = false;
                break;
            }
        }

        if(montageComplete){
            return;
            printf("Completou");
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}
