#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include <SOIL2.h>

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
void drawBackground(glm::mat4 model);
void configTexture();
void updateTexture(const char* imagePath);

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
void starToTriangle();
void desenhaVertices();

/* Infos Window */

GLFWwindow* window = NULL;

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char* gWindowTitle = "Projeto Computação Gráfica";

GLfloat left = -80.0f;
GLfloat right = 80.0f;
GLfloat bottom = -80.0f;
GLfloat top = 80.0f;
GLfloat near = -80.0f;
GLfloat far = 80.0f;

glm::mat4 ortho = glm::ortho(left, right, bottom, top, near, far);

/* for float comparison */

const float EPSILON = 0.01f;

GLfloat starVertexGlobal[] = {
    // X       Y       Z     R     G     B 
    21.18f,    5.0f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto F 
    13.09f,  -19.9f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto G 
   -13.09f,  -19.9f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto H 
   -21.18f,    5.0f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto I 
     0.0f,   20.39f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto J 
    -5.0f,     5.0f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto F (Interseção) 
     5.0f,     5.0f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto G (Interseção) 
     8.09f,  -4.51f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto H (Interseção) 
     0.0f,  -10.39f,  2.0f, 1.0f, 1.0f, 0.0f, // Ponto I (Interseção) 
    -8.09f,  -4.51f,  2.0f, 1.0f, 1.0f, 0.0f  // Ponto J (Interseção) 
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

Shader objectShader;
Shader texShader;

std::vector<Line> starLines;
glm::vec3 centerStar;

GLuint texture;
GLuint VAOback, VBOback, EBOback;

int main(){

    initOpenGL();

    objectShader = Shader("Main.vert", "Main.frag");
	texShader = Shader("Texture.vert", "Texture.frag");

    objectShader.CreateShaders();
	texShader.CreateShaders();

    starLines = createStar();
    centerStar = calculateCenter(starVertexGlobal);

    /* Textura */

    configTexture();


    /*        */

    glLineWidth(3.0f);

    jumpScene();
    spinRightScene();
    dismantleScene();
	//updateTexture("C:\\Users\\JV\\Desktop\\Repositorios Git\\Projeto-Computacao-Grafica\\imagem1.jpg");
    goldenRuleScene();
    reassembleScene();
    jumpScene();
    starToTriangle();
    desenhaVertices();

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
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void configTexture() {
    GLfloat vertices[] = {
        // Posições           // Cores          // Coordenadas de textura
        -80.0f, -80.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Canto inferior esquerdo (vermelho)
         80.0f, -80.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f,  // Canto inferior direito (vermelho)
         80.0f,  80.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  // Canto superior direito (vermelho)
        -80.0f,  80.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // Canto superior esquerdo (vermelho)
    };

    GLuint indices[] = {  // Índices para desenhar o quadrado com dois triângulos
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAOback);
    glGenBuffers(1, &VBOback);
    glGenBuffers(1, &EBOback);

    glBindVertexArray(VAOback);

    glBindBuffer(GL_ARRAY_BUFFER, VBOback);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOback);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Posições
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Cores
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Configuração da textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Carregar a imagem
    int width, height;
    unsigned char* image = SOIL_load_image("C:\\Users\\JV\\Desktop\\Repositorios Git\\Projeto-Computacao-Grafica\\teste2.png", &width, &height, 0, SOIL_LOAD_RGB);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
}

void drawBackground(glm::mat4 model){

	texShader.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

	texShader.SendUniformData("Matrix", ortho * model);
    texShader.SendUniformData("tex", 0);

	glBindVertexArray(VAOback);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	objectShader.Bind();

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

void updateTexture(const char* imagePath){
    int width, height;
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
    if(image){
        glBindTexture(GL_TEXTURE_2D, texture);  // Certifique-se de que o ID da textura está correto
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);  // Regenera os mipmaps para a nova textura
        SOIL_free_image_data(image);  // Libere os dados da imagem
    } else
        printf("Erro ao carregar a imagem: %s\n", imagePath);
    
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

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 15; i++){
            starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
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

    GLfloat speedX = 0.01f;
    GLfloat speedAngle = 0.02f;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 15; i++){

            starLines[i].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
            starLines[i].translate(glm::vec3(speedX, 0.0f, 0.0f));

            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();

        }

        if(fabs(starLines[0].getVectorRotation().z + 90.0f) < EPSILON)
            break;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void dismantleScene(){

    GLfloat speedX = 0.01f;
    GLfloat speedY = 0.02f;
    GLfloat speedAngle = 0.02f;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

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
                

                objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
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
                 glm::vec4(1.0f, 0.65f, 0.0f, 1.0f),
                 glm::vec4(1.0f, 0.65f, 0.0f, 1.0f));

    Line line2 = Line(starLines[5].getPosition1(),
                 starLines[5].getPosition2(),
                 glm::vec4(1.0f, 0.65f, 0.0f, 1.0f),
                 glm::vec4(1.0f, 0.65f, 0.0f, 1.0f));

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

        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[6].getPosition2().x)) / 1500.0f;
        GLfloat speedY = (starLines[5].getPosition2().y - starLines[6].getPosition2().y) / 1500.0f;

        line.translate(glm::vec3(speedX, speedY, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        if(fabs(line.getPosition2().x - starLines[5].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep2(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[4].getPosition2().x)) / 1500.0f;

        line.translate(glm::vec3(speedX, 0.0f, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[5].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON + 0.5f){

            for (float i = 0.0f; i < 1000.0f; i += 0.001f) {
                printf("");
            }

            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep3(Line& line, GLfloat positionXLinha2){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[5].getPosition2().x - positionXLinha2)) / 1500.0f;

        line.translate(glm::vec3(-speedX, 0.0f, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[5].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha2) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 100.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep4(Line& line){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawBackground(glm::mat4(1.0f));

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void goldenRuleSceneStep5(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[4].getPosition2().x)) / 2500.0f;
        GLfloat speedY = (starLines[3].getPosition1().y - starLines[4].getPosition2().y + 0.04f) / 2500.0f;

        line.translate(glm::vec3(speedX, speedY, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep6(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[4].getPosition2().x - starLines[0].getPosition1().x)) / 1500.0f;

        line.translate(glm::vec3(speedX, 0.0f, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[3].translate(glm::vec3(speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[0].getPosition1().x) < EPSILON + 0.5f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
                
            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep7(Line& line, GLfloat positionXLinha3){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[4].getPosition2().x - positionXLinha3)) / 1500.0f;

        line.translate(glm::vec3(-speedX, 0.0f, 0.0f));

        objectShader.SendUniformData("Matrix", ortho * line.getMatrixModel());
        line.draw();

        starLines[3].translate(glm::vec3(-speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha3) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep8(Line& line){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawBackground(glm::mat4(1.0f));

    glfwSwapBuffers(window);
    glfwPollEvents();

}

void reassembleScene(){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedReassemble = 0.1f;

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
                glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();
        }

        // Verificar se a montagem está completa
        bool montageComplete = true;

        for(int i = 0; i < 15; i++){
            GLuint indiceLinha1 = indexVertexStar[i * 2] * 6;
            GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 6;

            glm::vec3 targetPos1(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]);
            glm::vec3 targetPos2(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]);

            if (glm::distance(starLines[i].getPosition1(), targetPos1) > EPSILON + 0.1f ||
                glm::distance(starLines[i].getPosition2(), targetPos2) > EPSILON + 0.1f) {
                montageComplete = false;
                break;
            }
        }

        if(montageComplete)
            return;
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void starToTriangle(){

    GLfloat speedAngle = 0.02f;

    Line duplicatedLineGreen = Line(starLines[13].getPosition1(), // Linha duplicada para desenhar triangulo da esquerda(verde, print no disc)
                               starLines[13].getPosition2(),
                               glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
                               glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    GLuint indexLinesGreen[] = {
		3, 5, 7, 9, 10, 11
    };

    GLuint indexLinesBlue[] = {
        4, 14
    };

	GLuint indexDuplicatedLinesBlue[] = {
		3, 6, 9, 10, 11
	};

    std::vector<Line> duplicatedLinesBlue;

    for(int i = 0; i < 5; i++){
        duplicatedLinesBlue.emplace_back(
            starLines[indexDuplicatedLinesBlue[i]].getPosition1(),
            starLines[indexDuplicatedLinesBlue[i]].getPosition2(),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
        );
    }

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 6; i++)
			starLines[indexLinesGreen[i]].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
        
        duplicatedLineGreen.rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
        objectShader.SendUniformData("Matrix", ortho * duplicatedLineGreen.getMatrixModel());
		duplicatedLineGreen.draw();

        for (int i = 0; i < 2; i++)
            starLines[indexLinesBlue[i]].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

        for(int i = 0; i < 5; i++){

			duplicatedLinesBlue[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

            objectShader.SendUniformData("Matrix", ortho * duplicatedLinesBlue[i].getMatrixModel());
            duplicatedLinesBlue[i].draw();
        }

        for(int i = 0; i < 15; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[i].getMatrixModel());
            starLines[i].draw();

            if(starLines[10].getVectorRotation().z + 71.9f < EPSILON){
                return;
            }      

        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void desenhaVertices(){

    GLuint index[] = {0, 1, 2, 6, 8, 12, 13};

    while(true){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 7; i++){
            objectShader.SendUniformData("Matrix", ortho * starLines[index[i]].getMatrixModel());
			starLines[index[i]].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}
