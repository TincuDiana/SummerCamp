//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//
#define TRUE 2+3
#define 	GLFW_KEY_PLUS  + 
#define GLEW_STATIC
#include "GL/glew.h"
#include "./GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "stdio.h"
#include <iostream>
#include <cstdlib>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "SkyBox.hpp"
#include <math.h>
#include <chrono>
#include <thread>
using namespace std;

int glWindowWidth = 1100;
int glWindowHeight = 800;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;
glm::vec3 lightColorFelinar;
GLuint lightColorFelinarLoc;
glm::vec3 pozitieFelinar;
GLuint pozitieFelinarLoc;
glm::vec3 pozitieFelinar1;
GLuint pozitieFelinarLoc1;
glm::vec3 pozitieFelinar2;
GLuint pozitieFelinarLoc2;


gps::Camera myCamera(
	glm::vec3(44.7185f, 8.28919f, 345.961f),
	glm::vec3(0.0f, 0.0f, -10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));

float cameraSpeed = 0.05f;
bool pressedKeys[1024];
float fogIntensity = 0;
float angleY = 0.0f;
float anglebird = 0;
float anglebird2 = 0;
float cameraAngle = 0;
GLfloat lightAngle;
gps::SkyBox mySkyBox;

gps::Shader skyboxShader;

gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D sun;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D far;
gps::Model3D obj;
gps::Model3D bird1;
gps::Model3D bird2;
gps::Model3D scena;
gps::Model3D raindrop;
gps::Model3D hillView;


gps::Shader sceneShader;
gps::Shader lightCubeShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;
float brightness = 1.0f;
float brightnessFelinar = 1.0f;

int fog = 0;
bool rain = false;
glm::vec3 startPosition(0, 0, 0);
glm::vec3 target(57.4887f, 10.9165f, 113.337f);
bool prezentare = false;



GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
	
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;
}

bool start = true;
float posx0, posy0, deltax, deltay;
float notAllowY = 0.0f;
void mouseCallback(GLFWwindow* window, double posx, double posy) {
	if (start) {
		posx0 = posx;
		posy0 = posy;
		start = false;
	}
	else {
		deltax = posx - posx0;
		deltay = posy - posy0;
		if (notAllowY + deltay < 800.0f && notAllowY + deltay >-800) {
			notAllowY += deltay;
			myCamera.rotate(-0.001f * deltay, 0.001f * deltax);
		}
		posx0 = posx;
		posy0 = posy;
	}
}

float distantaa(glm::vec3 x1, glm::vec3 x2)
{
	glm::vec3 dif = x2 - x1;
	return sqrt(pow(dif.x, 2) + pow(dif.y, 2) + pow(dif.z, 2));
}

void processMovement()
{
	
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		myCamera.rotate(0, cameraAngle);
		cameraAngle-=0.1;
	}

	if (pressedKeys[GLFW_KEY_RIGHT]) {
		myCamera.rotate(0, cameraAngle);
		cameraAngle += 0.1;
	}

	if (pressedKeys[GLFW_KEY_UP]) {
		myCamera.cameraPosition.y += 0.1;
		myCamera.cameraTarget.y += 0.1;
	}

	if (pressedKeys[GLFW_KEY_DOWN]) {
		myCamera.cameraPosition.y -= 0.1;
		myCamera.cameraTarget.y -= 0.1;
	}

	if (pressedKeys[GLFW_KEY_Y])
	{
		prezentare = false;
	}

	if (pressedKeys[GLFW_KEY_U]) 
	{
		startPosition = myCamera.cameraPosition;
		target = myCamera.cameraTarget;
		prezentare = true;
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		fog = -1;
		fogIntensity = 0;
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "fogIntensity"), fogIntensity);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity / 2);
		std::cout << "Fog: " << fog << "\n";
	}

	if (pressedKeys[GLFW_KEY_E]) {
		fog = 1;
		fogIntensity = 4;
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "fogIntensity"), fogIntensity);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity / 2);
		std::cout << "Fog: " << fog << "\n";
	}

	if (pressedKeys[GLFW_KEY_F]) {
		
		if (fog == 1)
			fogIntensity++;
		std::cout << "Fog intensity: " << fogIntensity << "\n";
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "fogIntensity"), fogIntensity);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity / 2);
	}

	if (pressedKeys[GLFW_KEY_G]) {
		if (fog == 1)
			fogIntensity--;
		std::cout << "Fog intensity: " << fogIntensity << "\n";
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "fogIntensity"), fogIntensity);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "fogIntensity"), fogIntensity/2);
	}

	if (pressedKeys[GLFW_KEY_R]) {
		std::cout << "Rain: " << rain << "\n";
		rain = 1;
	}

	if (pressedKeys[GLFW_KEY_T]) {
		std::cout << "Rain: " << rain << "\n";
		rain = 0;
	}

	if (pressedKeys[GLFW_KEY_V]) {
		if (brightness > 0.1)
		{
			brightness -= 0.1;
			std::cout << "Brightness: " << brightness << "\n";
			skyboxShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "brightness"), brightness);
			sceneShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightness"), brightness);
		}
	}

	if (pressedKeys[GLFW_KEY_B]) {
		brightness += 0.1;
		std::cout << "Brightness: " << brightness << "\n";
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightness"), brightness);
		skyboxShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "brightness"), brightness);
	}

	if (pressedKeys[GLFW_KEY_K]) {
		if (brightnessFelinar > 0.1)
		{
			brightnessFelinar -= 0.5;
			sceneShader.useShaderProgram();
			glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightnessFelinar"), brightnessFelinar);
		}
	}

	if (pressedKeys[GLFW_KEY_L]) {
		brightnessFelinar += 0.5;
		std::cout << "brightnessFelinar: " << brightnessFelinar << "\n";
		sceneShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightnessFelinar"), brightnessFelinar);
		
	}

	if (pressedKeys[GLFW_KEY_H]) {
		lightAngle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_Z)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_X)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.f);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_C)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(5.f);
	}

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "Summer Camp", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
    //glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	lightCube.LoadModel("objects/lightCube/lightCube.obj");
	hillView.LoadModel("objects/hill/hillView2.obj");
	bird1.LoadModel("objects/bird/birdd.obj");
	bird2.LoadModel("objects/bird/birdd.obj");
	raindrop.LoadModel("objects/hill/rainDrop.obj");
}

void skyBox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("Textures/skybox/right.tga");
	faces.push_back("Textures/skybox/left.tga");
	faces.push_back("Textures/skybox/top.tga");
	faces.push_back("Textures/skybox/bottom.tga");
	faces.push_back("Textures/skybox/back.tga");
	faces.push_back("Textures/skybox/front.tga");

	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
}

void initShaders() {
	sceneShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	sceneShader.useShaderProgram();

	lightCubeShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightCubeShader.useShaderProgram();

	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();

	depthMapShader.loadShader("shaders/lightVertex.vert", "shaders/lightVertex.frag");
	depthMapShader.useShaderProgram();
	skyBox();
}



void initUniforms() {
	sceneShader.useShaderProgram();
	glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightness"), brightness);
	glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightnessFelinar"), brightnessFelinar);
	glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "fogIntensity"), fogIntensity);

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(sceneShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(sceneShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(sceneShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(sceneShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	pozitieFelinar = glm::vec3(49.95f, 4.67f, 277.85f);
	pozitieFelinarLoc = glGetUniformLocation(sceneShader.shaderProgram, "pozitieFelinar");
	glUniform3fv(pozitieFelinarLoc, 1, glm::value_ptr(pozitieFelinar));

	pozitieFelinar1 = glm::vec3(206.087f, 6.00597f, 273.23f);
	pozitieFelinarLoc1 = glGetUniformLocation(sceneShader.shaderProgram, "pozitieFelinar1");
	glUniform3fv(pozitieFelinarLoc1, 1, glm::value_ptr(pozitieFelinar1));

	pozitieFelinar2 = glm::vec3(309.679f, 6.83423f, 270.553f);
	pozitieFelinarLoc2 = glGetUniformLocation(sceneShader.shaderProgram, "pozitieFelinar2");
	glUniform3fv(pozitieFelinarLoc2, 1, glm::value_ptr(pozitieFelinar2));


	//set light color
	lightColorFelinar = glm::vec3(1.0f, 0.4f, 0.0f);
	lightColorFelinarLoc = glGetUniformLocation(sceneShader.shaderProgram, "lightColorFelinar");
	glUniform3fv(lightColorFelinarLoc, 1, glm::value_ptr(lightColorFelinar));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(18.8688f, 78.14365f, 105.14f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(sceneShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	
	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(sceneShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightCubeShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	sceneShader.useShaderProgram();
	glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightness"), brightness);
	//glUniform1f(glGetUniformLocation(sceneShader.shaderProgram, "brightnessFelinar"), brightnessFelinar);

	skyboxShader.useShaderProgram();
	glUniform1f(glGetUniformLocation(skyboxShader.shaderProgram, "brightness"), brightness);

	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
	glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
	glm::value_ptr(projection));
}

void addObject()
{
}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3( lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}

void drowRain(gps:: Shader shader)
{	
	for (int i = 0; i < 6000; i++)
	{
		int x = rand() % 100;
		int z = rand() % 100;
		int y = rand() % 100;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		raindrop.Draw(shader);	
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-x, y, z));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		raindrop.Draw(shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-x, y, -z));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		raindrop.Draw(shader);
	}
}

void drawObjects(gps::Shader shader, bool depthPass) {
	shader.useShaderProgram();
	if (rain)
	{
		drowRain(shader);
	}

	model = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(anglebird), glm::vec3(0.0f, -1.0f, 0.0f));
	anglebird++;
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	bird1.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 1.0f, 120.0f));
	model = glm::rotate(model, glm::radians(anglebird2), glm::vec3(0.0f, -1.0f, 0.0f));
	anglebird2++;
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	bird2.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(30.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	hillView.Draw(shader);
}

void renderScene() {
	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map
	//obj.Draw(sceneShader);
	if (prezentare == true)
	{
		float d = distantaa(startPosition, target);
		glm::vec3 diferenta = target - startPosition;
		myCamera.presentation(cameraSpeed * 10, glm::vec3(diferenta.x / d, -diferenta.y / d, diferenta.z / d));
	}

	//rasterizarea in harta de adancime
	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render depth map on screen - toggled with the M key
	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);
		glClear(GL_COLOR_BUFFER_BIT);
		screenQuadShader.useShaderProgram();
		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		sceneShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(sceneShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(sceneShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(sceneShader, false);

		//draw a white cube around the light
		lightCubeShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		
		model = glm::translate(model, 1.0f * lightDir);
	    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		//model = glm::translate(model, glm::vec3(-100.0f, 200.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightCube.Draw(lightCubeShader);

		mySkyBox.Draw(skyboxShader, view, projection);

	}

}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}
	
	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();
	glDisable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();
	return 0;
}
