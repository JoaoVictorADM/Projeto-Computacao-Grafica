#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
//#define _CRT_SECURE_NO_WARNINGS

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
#include "Triangle.h"
#include "globals.h"

/* Functions */

void initOpenGL();
std::vector<Line> createStar(glm::vec4 color);
std::vector<Triangle> createStarTriangles();
glm::vec3 calculateCenter(GLfloat *vertexVector);
void drawBackground(glm::mat4 model);
void configTexture();
void updateTexture(const char *imagePath);

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
void starToTriangle1(GLfloat angle, GLfloat speedAngle);
void starToTriangle2(GLfloat angle, GLfloat speedAngle);
void colorTriangles(glm::vec4 colorTriangles);
void openOrCloseTriangle(GLfloat angle);
void goldenRectangleScene();
void goldenRectangleSceneStep1(Triangle& triangle1, Triangle& triangle2, Line& line, GLuint* indexLines);
void goldenRectangleSceneStep2(Triangle& triangle1, Triangle& triangle2, Line& line, GLuint* indexLines);
void colorAllStar();
void jumpAllStarScene();
void jumpAllStarSceneStep1(GLfloat min, GLfloat max);
void jumpAllStarSceneStep2(GLfloat* min, GLfloat* max);
void changeColorTrianglesScene();
void edgesTranslation();
void trianglesToRectanglesScene();
void trianglesToRectanglesSceneStep1(std::vector<Triangle>& triangles);
void trianglesToRectanglesSceneStep2(GLfloat angle, std::vector<Triangle>& triangles);


/* Infos Window */

GLFWwindow* window = NULL;

const int gWindowWidth = 800;
const int gWindowHeight = 800;
const char* gWindowTitle = "Projeto Computação Gráfica";

GLfloat left = -80.0;
GLfloat right = 80.0;
GLfloat bottom = -80.0;
GLfloat top = 80.0f;
GLfloat near = -80.0;
GLfloat far = 80.0;

/* for float comparison */

const float EPSILON = 0.01f;

GLfloat starVertexGlobal[] = {
    // X       Y       Z    
    21.18f,    5.0f,  5.0f,  // Ponto F 0
    13.09f,  -19.9f,  5.0f,  // Ponto G 1
   -13.09f,  -19.9f,  5.0f,  // Ponto H 2
   -21.18f,    5.0f,  5.0f,  // Ponto I 3
     0.0f,   20.39f,  5.0f,  // Ponto J 4
    -5.0f,     5.0f,  5.0f,  // Ponto F (Interseção) 5
     5.0f,     5.0f,  5.0f,  // Ponto G (Interseção) 6
     8.09f,  -4.51f,  5.0f,  // Ponto H (Interseção) 7
     0.0f,  -10.39f,  5.0f,  // Ponto I (Interseção) 8
    -8.09f,  -4.51f,  5.0f,  // Ponto J (Interseção) 9
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

GLuint indexVextexStarTriangles[] = {
    9, 6, 5,  // 0
    9, 6, 7,  // 1
    9, 8, 7,  // 2
    3, 9, 5,  // 3
	0, 6, 7,  // 4
	4, 6, 5,  // 5
	2, 8, 9,  // 6
	1, 8, 7   // 7
};

std::vector<Line> starLines;
glm::vec3 centerStar;

std::vector<Triangle> starTriangles;

GLuint texture;
GLuint VAOback, VBOback, EBOback;

int main(){

    ortho = glm::ortho(left, right, bottom, top, near, far);

    initOpenGL();

    objectShader = Shader("Main.vert", "Main.frag");
	texShader = Shader("Texture.vert", "Texture.frag");

    objectShader.CreateShaders();
	texShader.CreateShaders();

    starLines = createStar(glm::vec4(1.0f, 0.8f, 0.7f, 1.0f));
    centerStar = calculateCenter(starVertexGlobal);

    /* Textura */

    configTexture();

    /*        */

    glLineWidth(3.0f);

    updateTexture("C:\\Users\\JV\\Desktop\\Repositorios Git\\Projeto-Computacao-Grafica\\background2.png");

    jumpScene();
    spinRightScene();
    dismantleScene();
    goldenRuleScene();
    reassembleScene();
    jumpScene();
    starToTriangle1(71.9f, 0.02f);
    colorTriangles(glm::vec4(1.0f, 0.41f, 0.70f, 0.0f));
    openOrCloseTriangle(54.0f);
    goldenRectangleScene();
    openOrCloseTriangle(-54.0f);
	colorTriangles(glm::vec4(0.12f, 0.56f, 1.0f, 0.0f));
    starToTriangle2(0.0f, -0.02f);
    colorAllStar();
    jumpAllStarScene();
    changeColorTrianglesScene();
    edgesTranslation();
    trianglesToRectanglesScene();

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
    unsigned char* image = SOIL_load_image("C:\\Users\\JV\\Desktop\\Repositorios Git\\Projeto-Computacao-Grafica\\background1.png", &width, &height, 0, SOIL_LOAD_RGB);
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

void updateTexture(const char* imagePath) {
    int width, height;
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
    if (image) {
        glBindTexture(GL_TEXTURE_2D, texture);  // Certifique-se de que o ID da textura está correto
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);  // Regenera os mipmaps para a nova textura
        SOIL_free_image_data(image);  // Libere os dados da imagem
    }
    else
        printf("Erro ao carregar a imagem: %s\n", imagePath);

}

std::vector<Line> createStar(glm::vec4 color){

    std::vector<Line> starLines;

    for(int i = 0; i < 15; i++){

        GLuint indiceLinha1 = indexVertexStar[i * 2] * 3;
        GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 3;

        starLines.emplace_back(
            glm::vec3(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]),
            glm::vec3(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]),
            color
        );

    }

    return starLines;
}

std::vector<Triangle> createStarTriangles(){

	std::vector<Triangle> starTriangles;

	for(int i = 0; i < 8; i++){

		GLuint indiceLinha1 = indexVextexStarTriangles[i * 3] * 3;
		GLuint indiceLinha2 = indexVextexStarTriangles[i * 3 + 1] * 3;
		GLuint indiceLinha3 = indexVextexStarTriangles[i * 3 + 2] * 3;

		starTriangles.emplace_back(
			glm::vec3(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2] - 1.0f),
			glm::vec3(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2] - 1.0f),
			glm::vec3(starVertexGlobal[indiceLinha3], starVertexGlobal[indiceLinha3 + 1], starVertexGlobal[indiceLinha3 + 2] - 1.0f),
			glm::vec4(1.0f, 0.41f, 0.70f, 0.0f)
		);

	}

	return starTriangles;

}

glm::vec3 calculateCenter(GLfloat* vertexVector){

    glm::vec3 center(0.0f, 0.0f, 0.0f);
    size_t numVertices = 10;

    for(size_t i = 0; i < numVertices; ++i){
        center += glm::vec3(vertexVector[i * 3], vertexVector[i * 3 + 1], vertexVector[i * 3 + 2]);
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


        for(int i = 0; i < 15; i++){

            if (((i == 3 || i == 4) && starLines[14].getVectorRotation().z < -17.90f) ||
                (i == 5 && starLines[14].getVectorRotation().z < 54.0f) ||
                (i == 6 && starLines[14].getVectorRotation().z < 126.0f) ||
                (i >= 7)
                ) {

                starLines[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
                starLines[i].translate(glm::vec3(-speedX, 0.0f, 0.0f));

            } else {
                starLines[i].setColor(glm::vec4(1.0f, 0.71f, 0.76f, 1.0f));
            }

            if(i == 6 && starLines[14].getVectorRotation().z >= 180.0f && starLines[6].getPosition2().y >= starLines[0].getPosition2().y)
                starLines[i].translate(glm::vec3(0.0f, -speedY, 0.0f));
                

            if(starLines[14].getVectorRotation().z >= 220.0f)
                return;
                
            starLines[i].draw();
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
                 glm::vec4(1.0f, 0.71f, 0.76f, 0.4f));
                 

    Line line2 = Line(starLines[5].getPosition1(),
                 starLines[5].getPosition2(),
                 glm::vec4(1.0f, 0.71f, 0.76f, 0.4f));

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

        line.draw();

        if(fabs(line.getPosition2().x - starLines[5].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

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

        line.draw();

        starLines[5].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON + 0.5f){

            for (float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");

            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

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

        line.draw();

        starLines[5].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha2) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 100.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep4(Line& line){

	glm::vec4 colorLine = line.getColor();

    while(true){
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

		colorLine.a -= 0.0003f;

		if(colorLine.a <= 0.0f)
			break;

		line.setColor(colorLine);
        line.draw();

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        
}

void goldenRuleSceneStep5(Line& line){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        drawBackground(glm::mat4(1.0f));

        GLfloat speedX = fabs((starLines[5].getPosition2().x - starLines[4].getPosition2().x)) / 2500.0f;
        GLfloat speedY = (starLines[3].getPosition1().y - starLines[4].getPosition2().y + 0.04f) / 2500.0f;

        line.translate(glm::vec3(speedX, speedY, 0.0f));

        line.draw();

        if(fabs(line.getPosition2().x - starLines[4].getPosition2().x) < EPSILON){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f) 
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

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

        line.draw();

        starLines[3].translate(glm::vec3(speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - starLines[0].getPosition1().x) < EPSILON + 0.5f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
                
            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

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

        line.draw();

        starLines[3].translate(glm::vec3(-speedX, 0.0f, 0.0f));
        starLines[4].translate(glm::vec3(-speedX, 0.0f, 0.0f));

        if(fabs(line.getPosition2().x - positionXLinha3) < EPSILON + 0.01f){

            for(float i = 0.0f; i < 1000.0f; i += 0.001f)
                printf("");
            
            break;

        }

        for(int i = 0; i < 7; i++)
            starLines[i].draw();
        

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRuleSceneStep8(Line& line){

    glm::vec4 colorLine = line.getColor();

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        colorLine.a -= 0.0003f;

        if (colorLine.a <= 0.0f)
            break;

        line.setColor(colorLine);
        line.draw();

        for (int i = 0; i < 7; i++)
            starLines[i].draw();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void reassembleScene(){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        GLfloat speedReassemble = 0.1f;

        // Montar as linhas restantes do pentagrama
        for(int i = 0; i < 15; i++){

            GLuint indiceLinha1 = indexVertexStar[i * 2] * 3;
            GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 3;

            glm::vec3 targetPos1(starVertexGlobal[indiceLinha1], starVertexGlobal[indiceLinha1 + 1], starVertexGlobal[indiceLinha1 + 2]);
            glm::vec3 targetPos2(starVertexGlobal[indiceLinha2], starVertexGlobal[indiceLinha2 + 1], starVertexGlobal[indiceLinha2 + 2]);

            glm::vec3 currentPos1 = starLines[i].getPosition1();
            glm::vec3 currentPos2 = starLines[i].getPosition2();

            glm::vec3 direction1 = glm::normalize(targetPos1 - currentPos1);
            glm::vec3 direction2 = glm::normalize(targetPos2 - currentPos2);

            glm::vec3 newPos1 = currentPos1 + direction1 * (float)speedReassemble;
            glm::vec3 newPos2 = currentPos2 + direction2 * (float)speedReassemble;

            // Atualizar a posição da linha
            starLines[i] = Line(newPos1, newPos2,
                glm::vec4(1.0f, 0.71f, 0.76f, 1.0f));

            starLines[i].draw();
        }

        // Verificar se a montagem está completa
        bool montageComplete = true;

        for(int i = 0; i < 15; i++){
            GLuint indiceLinha1 = indexVertexStar[i * 2] * 3;
            GLuint indiceLinha2 = indexVertexStar[i * 2 + 1] * 3;

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

void starToTriangle1(GLfloat angle, GLfloat speedAngle){

	starTriangles = createStarTriangles();

    Line duplicatedLineGreen = Line(starLines[13].getPosition1(), // Linha duplicada para desenhar triangulo da esquerda(verde, print no disc)
                               starLines[13].getPosition2(),
                               glm::vec4(1.0f, 0.71f, 0.76f, 1.0f));

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
            glm::vec4(1.0f, 0.71f, 0.76f, 1.0f)
        );
    }

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 6; i++)
			starLines[indexLinesGreen[i]].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
        
        duplicatedLineGreen.rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
		duplicatedLineGreen.draw();

        for (int i = 0; i < 2; i++)
            starLines[indexLinesBlue[i]].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

        for(int i = 0; i < 5; i++){

			duplicatedLinesBlue[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

            duplicatedLinesBlue[i].draw();
        }

        for(int i = 0; i < 15; i++){
            starLines[i].draw();

            if(fabs(starLines[10].getVectorRotation().z + angle) < EPSILON){
                return;
            }      

        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void starToTriangle2(GLfloat angle, GLfloat speedAngle) {

    for (int i = 0; i < 15; i++)
        starLines[i].setColor(glm::vec4(0.53f, 0.80f, 1.0f, 1.0f));

    Line duplicatedLineGreen = Line(starLines[6].getPosition1(), // Linha duplicada para desenhar triangulo da esquerda(verde, print no disc)
        starLines[6].getPosition2(),
        starLines[6].getColor());

    GLuint indexLinesGreen[] = {
        3, 5, 7, 9, 10, 11
    };

    GLuint indexLinesBlue[] = {
        4, 14
    };

    GLuint indexDuplicatedLinesBlue[] = {
        1, 2, 6, 8, 13 
    };

    std::vector<Line> duplicatedLinesBlue;

    for (int i = 0; i < 5; i++) {
        duplicatedLinesBlue.emplace_back(
            starLines[indexDuplicatedLinesBlue[i]].getPosition1(),
            starLines[indexDuplicatedLinesBlue[i]].getPosition2(),
            starLines[indexDuplicatedLinesBlue[i]].getColor()
        );
    }

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for (int i = 0; i < 5; i++)
            starTriangles[i].draw();
        

        for (int i = 0; i < 6; i++)
            starLines[indexLinesGreen[i]].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

        duplicatedLineGreen.rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);
        duplicatedLineGreen.draw();

        for (int i = 0; i < 2; i++)
            starLines[indexLinesBlue[i]].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

        for (int i = 0; i < 5; i++) {

            duplicatedLinesBlue[i].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centerStar);

            duplicatedLinesBlue[i].draw();
        }

        for (int i = 0; i < 15; i++) {
            starLines[i].draw();

            if (fabs(starLines[10].getVectorRotation().z + angle) < EPSILON) {
                return;
            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void colorTriangles(glm::vec4 colorTriangles){

    for(int i = 0; i < 8; i++)
        starTriangles[i].setColor(colorTriangles);
    
    GLuint indexLines[] = { 0, 1, 2, 6, 8, 12, 13 };

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));


        for(int i = 0; i < 5; i++){

            glm::vec4 triangleColor = starTriangles[i].getColor();

            triangleColor.a += 0.0003f;

            starTriangles[i].setColor(triangleColor);
            starTriangles[i].draw();

            if(triangleColor.a >= 1.0f)
                return;

        }

        for(int i = 0; i < 7; i++)
            starLines[indexLines[i]].draw();
        

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void colorAllStar(){

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

       
        for(int i = 0; i < 8; i++) {
            
            glm::vec4 triangleColor = starTriangles[i].getColor();

            if (i >= 5){

                triangleColor.a += 0.0003f;

                starTriangles[i].setColor(triangleColor);

                if(triangleColor.a > 1.0f)
                    return;
            }

            starTriangles[i].draw();


        }

        for (int i = 0; i < 15; i++)
            starLines[i].draw();


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void openOrCloseTriangle(GLfloat angle){

    GLuint indexLines[] = { 0, 1, 2, 6, 8, 12, 13 };
    GLuint indexLinesMove[] = { 6, 8, 12, 13 };

    int steps = 1000;

    GLfloat speedAngle = angle / (GLfloat)steps;

    for(int i = 0; i < steps; i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for (int i = 0; i < 5; i++)
            starTriangles[i].draw();

        for(int i = 0; i < 2; i++)
            starLines[indexLinesMove[i]].rotate(-speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), starLines[8].getPosition2());
           
        for(int i = 2; i < 4; i++)
            starLines[indexLinesMove[i]].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), starLines[0].getPosition2());
 
        for (int i = 0; i < 7; i++)
            starLines[indexLines[i]].draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void goldenRectangleScene(){

    GLuint indexLines[] = { 0, 1, 2, 6, 8, 12, 13 };

    for(int i = 0; i < 7; i++)
        starLines[indexLines[i]].setColor(glm::vec4(1.0f, 0.8f, 0.7f, 1.0f));

	Line line = Line(starLines[6].getPosition1(),
		        starLines[13].getPosition2(),
                glm::vec4(1.0f, 0.8f, 0.7f, 0.0f));

	glm::vec3 position1Triangle1 = starLines[8].getPosition2();
	glm::vec3 position2Triangle1 = starLines[12].getPosition1();
	glm::vec3 position3Triangle1 = starLines[6].getPosition1();

	position1Triangle1.z -= 2.0f;
	position2Triangle1.z -= 2.0f;
	position3Triangle1.z -= 2.0f;

	glm::vec3 position1Triangle2 = starLines[12].getPosition1();
	glm::vec3 position2Triangle2 = starLines[6].getPosition1();
	glm::vec3 position3Triangle2 = starLines[13].getPosition2();

	position1Triangle2.z -= 2.0f;
	position2Triangle2.z -= 2.0f;
	position3Triangle2.z -= 2.0f;

	Triangle triangle1 = Triangle(position1Triangle1,
		                 position2Triangle1,
		                 position3Triangle1,
		                 glm::vec4(1.0f, 1.0f, 0.4f, 0.0f));

    Triangle triangle2 = Triangle(position1Triangle2,
                         position2Triangle2,
                         position3Triangle2,
                         glm::vec4(1.0f, 1.0f, 0.4f, 0.0f));

	goldenRectangleSceneStep1(triangle1, triangle2, line, indexLines);
    goldenRectangleSceneStep2(triangle1, triangle2, line, indexLines);

}

void goldenRectangleSceneStep1(Triangle& triangle1, Triangle& triangle2, Line& line, GLuint *indexLines){

    for(float i = 0; i < 1; i += 0.0001f) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

		glm::vec4 colorTriangle1 = triangle1.getColor();
		colorTriangle1.a = i ;
		triangle1.setColor(colorTriangle1);
        triangle1.draw();

        glm::vec4 colorTriangle2 = triangle2.getColor();
        colorTriangle2.a = i;
        triangle2.setColor(colorTriangle2);
		triangle2.draw();

		glm::vec4 colorLine = line.getColor();
		colorLine.a = i;
		line.setColor(colorLine);
        line.draw();

		for(int j = 0; j < 7; j++)
			starLines[indexLines[j]].draw();

        for(int j = 0; j < 5; j++){
			glm::vec4 triangleColor = starTriangles[j].getColor();
			triangleColor.a = 1.0f - i;
			starTriangles[j].setColor(triangleColor);
			starTriangles[j].draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
}

void goldenRectangleSceneStep2(Triangle& triangle1, Triangle& triangle2, Line& line, GLuint* indexLines){

    GLfloat alfaLimit = 1.0f;
    
	// preciso mover o alfa para 0 e subir para um 6 vezes, mas o alfa limit diminui em 0,2 a cada vez

	for(int i = 0; i < 5; i++){

		for(float j = alfaLimit; j >= 0.2f; j -= ((alfaLimit - 0.2f) / 2500.0f)) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawBackground(glm::mat4(1.0f));

			glm::vec4 colorTriangle1 = triangle1.getColor();
			colorTriangle1.a = j;
			triangle1.setColor(colorTriangle1);
			triangle1.draw();

			glm::vec4 colorTriangle2 = triangle2.getColor();
			colorTriangle2.a = j;
			triangle2.setColor(colorTriangle2);
			triangle2.draw();

			line.draw();

			for (int k = 0; k < 7; k++)
				starLines[indexLines[k]].draw();

			glfwSwapBuffers(window);
			glfwPollEvents();

		}

        alfaLimit -= 0.16f;

        for(float j = 0.2f + EPSILON; j < alfaLimit; j += ((alfaLimit - 0.2f) / 2500.0f)) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawBackground(glm::mat4(1.0f));

			glm::vec4 colorTriangle1 = triangle1.getColor();
			colorTriangle1.a = j;
			triangle1.setColor(colorTriangle1);
			triangle1.draw();

			glm::vec4 colorTriangle2 = triangle2.getColor();
			colorTriangle2.a = j;
			triangle2.setColor(colorTriangle2);
			triangle2.draw();

			line.draw();

			for(int k = 0; k < 7; k++)
				starLines[indexLines[k]].draw();

			glfwSwapBuffers(window);
			glfwPollEvents();

		}

	}

    GLfloat alfa = triangle1.getColor().a;

    for(float i = alfa; i > 0.0f; i -= alfa / 5000.0f){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        glm::vec4 colorTriangle1 = triangle1.getColor();
        colorTriangle1.a = i;
        triangle1.setColor(colorTriangle1);
        triangle1.draw();

        glm::vec4 colorTriangle2 = triangle2.getColor();
        colorTriangle2.a = i;
        triangle2.setColor(colorTriangle2);
        triangle2.draw();

        line.draw();

        for(int k = 0; k < 7; k++)
            starLines[indexLines[k]].draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    for(float i = line.getColor().a; i > 0.0f; i -= 1.0f / 1000.f){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        glm::vec4 lineColor = line.getColor();
		lineColor.a = i;
        line.setColor(lineColor);
        line.draw();

        for (int k = 0; k < 7; k++)
            starLines[indexLines[k]].draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void jumpAllStarScene(){

	GLfloat min = 0.0f;
    GLfloat max = 20.0f;

    jumpAllStarSceneStep1(min, max);
    jumpAllStarSceneStep2(&min, &max);
    jumpAllStarSceneStep1(min, max);

}

void jumpAllStarSceneStep1(GLfloat min, GLfloat max){

    GLuint jumps = 2;

    GLdouble speedY = (max - min) / 2000.0;

    while(true){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int i = 0; i < 15; i++) {
            starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            starLines[i].draw();

        }

        for(int i = 0; i < 8; i++){
            starTriangles[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            starTriangles[i].draw();
        }

        if (starLines[0].getVectorTranslation().y <= min || starLines[0].getVectorTranslation().y >= max) {

            speedY *= -1.0f;

            if (starLines[0].getVectorTranslation().y <= min) {

                jumps--;

                if (jumps == 0)
                    break;

            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void jumpAllStarSceneStep2(GLfloat* min, GLfloat* max){

	GLuint jumps = 2;
    GLdouble scale = 1.0;
    GLdouble speedScale = 0.2/16000.0;
    GLdouble speedY = 0.01;

    while(true){
        
        GLdouble jumpHeight = (20.0 - ((20.0 - 0.0) * scale)) / 2.0;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for (int i = 0; i < 15; i++){
			starLines[i].scale(glm::vec3(-speedScale, -speedScale, 0.0f));
            starLines[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            starLines[i].draw();
        }

        for(int i = 0; i < 8; i++) {
            starTriangles[i].scale(glm::vec3(-speedScale, -speedScale, 0.0f));
            starTriangles[i].translate(glm::vec3(0.0f, speedY, 0.0f));
            starTriangles[i].draw();
        }

		scale -= speedScale;
        *min = 0.0f + (GLfloat)jumpHeight;
        *max = 20.0f - (GLfloat)jumpHeight;
        
        if(starLines[0].getVectorTranslation().y <= *min || starLines[0].getVectorTranslation().y >= *max) {

            speedY *= -1.0f;

            if (starLines[0].getVectorTranslation().y <= *min) {

                jumps--;

                if (jumps == 0)
                    break;

            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void changeColorTrianglesScene(){

    int steps = 2000;
	GLfloat stepRed = (0.53f - 0.12f) / (GLfloat)steps;
	GLfloat stepGreen = (0.80f - 0.56f) / (GLfloat)steps;

    for(int i = 0; i < steps; i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int j = 0; j < 15; j++)
            starLines[j].draw();
        

		for(int j = 0; j < 8; j++){

            if(j >= 3){
                glm::vec4 color = starTriangles[j].getColor();
                color.r += stepRed;
                color.g += stepGreen;
                starTriangles[j].setColor(color);
            }

			
			starTriangles[j].draw();

		}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void edgesTranslation(){

    GLuint pentagonLines[] = {1, 3, 13, 6, 9};
    GLuint trianglesIndex[] = { 5, 4, 7, 6, 3 };
    int steps = 2000;

    glm::vec3 distances[5] = {};

    GLuint moveLines[] = { 1, 10, 11,  3, 0, 12,  13, 4, 14,  6, 5, 7,  9, 8, 2 };

    for(int i = 0; i < 5; i++){

		glm::vec3 midpoint = (starLines[pentagonLines[i]].getPosition1() + starLines[pentagonLines[i]].getPosition2()) / 2.0f;

        distances[i] = ((starTriangles[trianglesIndex[i]].getPosition1() + midpoint) / 2.0f) - midpoint;
    }

    for(int i = 0; i < steps; i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int j = 0; j < 5; j++) {

            glm::vec3 translation = glm::vec3(distances[j].x / (GLfloat)steps, distances[j].y / (GLfloat)steps, 0.0f);

			starLines[moveLines[j * 3]].translate(translation);
            starLines[moveLines[j * 3 + 1]].translate(translation);
            starLines[moveLines[j * 3 + 2]].translate(translation);

        }
        
        for(int j = 0; j < 8; j++)
			starTriangles[j].draw();

		for (int j = 0; j < 15; j++)
			starLines[j].draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}

void trianglesToRectanglesScene(){

    std::vector<Triangle> duplicatedTriangles;

	glm::vec3 positions[] = {
		starLines[1].getPosition1(),
        starLines[10].getPosition2(),
        starLines[1].getPosition2(),

        starLines[3].getPosition1(),
        starLines[3].getPosition2(),
        starLines[0].getPosition2(),

        starLines[13].getPosition1(),
        starLines[13].getPosition2(),
        starLines[4].getPosition2(),

        starLines[6].getPosition1(),
        starLines[6].getPosition2(),
        starLines[7].getPosition1(),

        starLines[9].getPosition1(),
        starLines[9].getPosition2(),
        starLines[8].getPosition2()
	};

	glm::vec4 color = starTriangles[7].getColor();
    color.a = 0.0f;

    for(int i = 0; i < 5; i++) {
        duplicatedTriangles.emplace_back(
            positions[i * 3],
            positions[i * 3 + 1],
            positions[i * 3 + 2],
            color
        );
    }

	trianglesToRectanglesSceneStep1(duplicatedTriangles);
    trianglesToRectanglesSceneStep2(18.0f, duplicatedTriangles);

}

void trianglesToRectanglesSceneStep1(std::vector<Triangle> &triangles){

	int steps = 4000;

    for(int i = 0; i < steps; i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));


		glm::vec4 colorDuplicatedTriangles = triangles[0].getColor();
        colorDuplicatedTriangles.a += 1.0f / (GLfloat)steps;

		glm::vec4 colorTriangles = starTriangles[7].getColor();
		colorTriangles.a -= 1.0f / (GLfloat)steps;

        for(int j = 0; j < 8; j++){
            
            if(j >= 3)
				starTriangles[j].setColor(colorTriangles);

            if (j < 5){
				triangles[j].setColor(colorDuplicatedTriangles);
				triangles[j].draw();
            }

			starTriangles[j].draw();

        }

        for(int j = 0; j < 15; j++)
			starLines[j].draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void trianglesToRectanglesSceneStep2(GLfloat angle, std::vector<Triangle>& triangles){

    GLuint indexLinesMove[] = {10, 11,  0, 12,  4, 14,  5, 7,  8, 2};

    int steps = 1;

    GLfloat speedAngle = angle / (GLfloat)steps;

    GLuint indexCenters[] = { 1, 3, 13, 6, 9 };

    glm::vec3 centers[10] = {

		starTriangles[0].getPosition3(),
        starTriangles[0].getPosition2(),

        starTriangles[0].getPosition2(),
        starTriangles[1].getPosition3(),

        starTriangles[1].getPosition3(),
        starTriangles[2].getPosition2(),

        starTriangles[2].getPosition2(),
        starTriangles[2].getPosition1(),

        starTriangles[2].getPosition1(),
        starTriangles[0].getPosition3(),
        
    };

    for(int i = 0; i < steps; i++){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawBackground(glm::mat4(1.0f));

        for(int j = 0; j < 3; j++)
            starTriangles[j].draw();

        for(int j = 0; j < 5; j++)
            triangles[j].draw();

        for(int j = 0; j < 10; j++){

			starLines[indexLinesMove[j]].rotate(speedAngle, glm::vec3(0.0f, 0.0f, 1.0f), centers[j]);
			starLines[indexLinesMove[j]].draw();

            speedAngle = -speedAngle;
        }

        for(int j = 0; j < 15; j++)
            starLines[j].draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    while (true);
}


