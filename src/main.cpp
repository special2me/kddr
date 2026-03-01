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

Camera camera(glm::vec3(0.f, 10.f, -30.f));

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
	Model s1Model("stages/(2014) BOOM BLUE/Map_A1.txt", imp);
	Model s2Model("stages/(2014) BOOM BLUE/Map_A2.txt", imp);
	Model s3Model("stages/(2014) BOOM BLUE/Map_B.txt", imp);
	Model s4Model("stages/(2014) BOOM BLUE/Map_C1.txt", imp);
	Model s5Model("stages/(2014) BOOM BLUE/Map_C3.txt", imp);
	Model s6Model("stages/(2014) BOOM BLUE/Map_C4.txt", imp);
	Model s7Model("stages/(2014) BOOM BLUE/Map_D.txt", imp);
	Model s8Model("stages/(2014) BOOM BLUE/Map_E.txt", imp);
	Model s9Model("stages/(2014) BOOM BLUE/Map_F.txt", imp);
	Model s0Model("stages/(2014) BOOM BLUE/Map_F3.txt", imp);
	if (!pModel.is_ready() || !s1Model.is_ready() || !s2Model.is_ready()) return -1;
	if (!s3Model.is_ready()) return -1;
	if (!s4Model.is_ready()) return -1;
	if (!s5Model.is_ready()) return -1;
	if (!s6Model.is_ready()) return -1;
	if (!s7Model.is_ready()) return -1;
	if (!s8Model.is_ready()) return -1;
	if (!s9Model.is_ready()) return -1;
	if (!s0Model.is_ready()) return -1;

	Animation pAnimation("dances/R Saturation.txt", imp, 30.f);
	Animation s4Animation("stages/(2014) BOOM BLUE/Map_C1.txt", imp, 30.f);
	Animation s5Animation("stages/(2014) BOOM BLUE/Map_C3.txt", imp, 30.f);
	Animation s6Animation("stages/(2014) BOOM BLUE/Map_C4.txt", imp, 30.f);
	Animation s7Animation("stages/(2014) BOOM BLUE/Map_D.txt", imp, 30.f);
	Animation s9Animation("stages/(2014) BOOM BLUE/Map_F.txt", imp, 30.f);
	Animation s0Animation("stages/(2014) BOOM BLUE/Map_F3.txt", imp, 30.f);
	if (!pAnimation.is_ready()) return -1;
	if (!s4Animation.is_ready()) return -1;
	if (!s5Animation.is_ready()) return -1;
	if (!s6Animation.is_ready()) return -1;
	if (!s7Animation.is_ready()) return -1;
	if (!s9Animation.is_ready()) return -1;
	if (!s0Animation.is_ready()) return -1;

	Animator animator(&pAnimation);
	Animator s4animator(&s4Animation);
	Animator s5animator(&s5Animation);
	Animator s6animator(&s6Animation);
	Animator s7animator(&s7Animation);
	Animator s9animator(&s9Animation);
	Animator s0animator(&s0Animation);

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
		s4animator.update(deltaTime);
		s5animator.update(deltaTime);
		s6animator.update(deltaTime);
		s7animator.update(deltaTime);
		s9animator.update(deltaTime);
		s0animator.update(deltaTime);
		processInput();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera.getProjectionMatrix(screenWidth, screenHeight);
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		std::vector<glm::mat4> boneMatricies = animator.getBoneMatricies();
		for (size_t i = 0; i < boneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", glm::mat4(1.f));
		}

		s1Model.draw(shader);
		s2Model.draw(shader);
		s3Model.draw(shader);

		std::vector<glm::mat4> s4BoneMatricies = s4animator.getBoneMatricies();
		for (size_t i = 0; i < s4BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s4BoneMatricies[i]);
		}
		s4Model.draw(shader);

		std::vector<glm::mat4> s5BoneMatricies = s5animator.getBoneMatricies();
		for (size_t i = 0; i < s5BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s5BoneMatricies[i]);
		}
		s5Model.draw(shader);

		std::vector<glm::mat4> s6BoneMatricies = s6animator.getBoneMatricies();
		for (size_t i = 0; i < s6BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s6BoneMatricies[i]);
		}
		s6Model.draw(shader);

		std::vector<glm::mat4> s7BoneMatricies = s7animator.getBoneMatricies();
		for (size_t i = 0; i < s7BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s7BoneMatricies[i]);
		}
		s7Model.draw(shader);

		std::vector<glm::mat4> s9BoneMatricies = s9animator.getBoneMatricies();
		for (size_t i = 0; i < s9BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s9BoneMatricies[i]);
		}
		//s9Model.draw(shader);

		std::vector<glm::mat4> s0BoneMatricies = s0animator.getBoneMatricies();
		for (size_t i = 0; i < s0BoneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", s0BoneMatricies[i]);
		}
		//s0Model.draw(shader);


		//toggleDrawMode();

		for (size_t i = 0; i < boneMatricies.size(); i++) {
			shader.setMat4("boneMatricies[" + std::to_string(i) + "]", boneMatricies[i]);
		}
		pModel.draw(shader);
		//toggleDrawMode();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.close();

	glfwTerminate();

	return 0;
}

