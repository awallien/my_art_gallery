#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>

#include <Windows.h>

#include "ShaderConfig.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint transLoc, scaleLoc, thetaLoc, cPositionLoc, cLookAtLoc, cUpLoc, tfLoc;
GLuint leftLoc, rightLoc, topLoc, bottomLoc, nearLoc, farLoc;
int width, height;

void setupVertices(void) {
    float vertexPositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    ShaderInfo info[2] = {
        {GL_VERTEX_SHADER, "crazy.vert"},
        {GL_FRAGMENT_SHADER, "crazy.frag"}
    };
    renderingProgram = loadShader(info, 2);
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f;
    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    
    glUseProgram(renderingProgram);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    cubeLocX = sin(0.35f * currentTime) * 2.0f;
    cubeLocY = cos(0.52f * currentTime) * 2.0f;
    cubeLocZ = sin(0.7f * currentTime) * 2.0f;
    
    transLoc = glGetUniformLocation(renderingProgram, "trans");
    scaleLoc = glGetUniformLocation(renderingProgram, "scale");
    cPositionLoc = glGetUniformLocation(renderingProgram, "cPosition");
    cLookAtLoc = glGetUniformLocation(renderingProgram, "cLookAt");
    cUpLoc = glGetUniformLocation(renderingProgram, "cUp");
    thetaLoc = glGetUniformLocation(renderingProgram, "theta");
    tfLoc = glGetUniformLocation(renderingProgram, "tf");

    topLoc = glGetUniformLocation(renderingProgram, "top");
    bottomLoc = glGetUniformLocation(renderingProgram, "bottom");
    leftLoc = glGetUniformLocation(renderingProgram, "left");
    rightLoc = glGetUniformLocation(renderingProgram, "right");
    farLoc = glGetUniformLocation(renderingProgram, "far");
    nearLoc = glGetUniformLocation(renderingProgram, "near"); 
 
    glUniform3f(transLoc, cubeLocX, cubeLocY, cubeLocZ);
    glUniform3f(scaleLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(cPositionLoc, cameraX, cameraY, cameraZ+500.0f);
    glUniform3f(cLookAtLoc, 20.0f, 0.0f, 0.0f);
    glUniform3f(cUpLoc, 1.0f, 1.0f, 0.0f);
    glUniform3f(thetaLoc, 10.0f, 10.0f, 10.0f);
    glUniform1f(topLoc, 1.0f);
    glUniform1f(bottomLoc, -1.0f);
    glUniform1f(leftLoc, -1.0f);
    glUniform1f(rightLoc, 1.0f);
    glUniform1f(farLoc, 1000.0f);
    glUniform1f(nearLoc, 5.5f);
    glUniform1f(tfLoc, currentTime);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, (GLint)10000);
}


int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Infinity Cube", NULL, NULL);
    int maj = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int min = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if (maj < 3 || (maj == 3 && min < 2)) {
        cerr << "*** GLSL 1.50 shaders may not compile" << endl;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        Sleep(5);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
