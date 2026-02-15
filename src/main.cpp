#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "logger.h"
#include "camera.h"
#include "ms3dAsciiParser.h"
#include "model.h"
#include "animation.h"
#include "animator.h"
#include "shader.h"

int screenWidth = 1280;
int screenHeight = 720;

Camera camera(glm::vec3(0.f, 10.f, 30.f));

float lastScrX = screenWidth / 2.f;
float lastScrY = screenHeight / 2.f;

float deltaTime = 0.f;
float lastFrame = 0.f;

bool wireframe = false;

bool cameraFwd = false;
bool cameraBwd = false;
bool cameraLft = false;
bool cameraRgt = false;
bool dirtyQ = false;

void updateDrawMode() {
	GLenum mode = wireframe ? GL_LINE : GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void toggleDrawMode() {
	wireframe = !wireframe;
	updateDrawMode();
}

void key_callback(GLFWwindow *window, int key, int /* scancode */, int action, int /* mods */) {
	switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_W:
			if (action == GLFW_PRESS) cameraFwd = true;
			else if (action == GLFW_RELEASE) cameraFwd = false;
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS) cameraBwd = true;
			else if (action == GLFW_RELEASE) cameraBwd = false;
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS) cameraLft = true;
			else if (action == GLFW_RELEASE) cameraLft = false;
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS) cameraRgt = true;
			else if (action == GLFW_RELEASE) cameraRgt = false;
			break;
		case GLFW_KEY_Q:
			if (action == GLFW_PRESS) toggleDrawMode();
			break;
		default:
			break;
	}
}

void mouse_callback(GLFWwindow * /* window */, double xposD, double yposD) {
	float xpos = static_cast<float>(xposD);
	float ypos = static_cast<float>(yposD);

	float xoff = xpos - lastScrX;
	float yoff = ypos - lastScrY;

	lastScrX = xpos;
	lastScrY = ypos;

	camera.updateRotation(xoff, -yoff);
}

void processInput() {
	if (cameraFwd) camera.updatePosition(Camera::FORWARD, deltaTime);
	if (cameraBwd) camera.updatePosition(Camera::BACKWARDS, deltaTime);
	if (cameraLft) camera.updatePosition(Camera::LEFT, deltaTime);
	if (cameraRgt) camera.updatePosition(Camera::RIGHT, deltaTime);
}

int main() {
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "ddddddance dance revolution", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("GLAD failed to load");
		glfwTerminate();
		return -1;
	}

	Ms3dAsciiParser imp;
	//Model pModel("models/spec/MilkShape 3D ASCII.txt", imp);
	Model pModel("models/(2nd) Dred/model.txt", imp);
	if (!pModel.is_ready()) return -1;

	Animation pAnimation("models/DanceRepo/R Saturation.txt", imp);
	if (!pAnimation.is_ready()) return -1;

	Animator animator(&pAnimation);

	Shader shader("shaders/render.vs", "shaders/render.fs");
	if (!shader.is_ready()) {
		glfwTerminate();
		return -1;
	}

	shader.use();
	updateDrawMode();
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		animator.update(deltaTime);
		processInput();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera.getProjectionMatrix(screenWidth, screenHeight);
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		std::vector<glm::mat4> boneMatricies = animator.getBoneMatricies();
		for (size_t i = 0; i < boneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", boneMatricies[i]);
		}

		pModel.draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.close();

	glfwTerminate();

	return 0;
}

