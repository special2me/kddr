#include "camera.h"

void Camera::updatePosition(Movement direction, float deltaTime) {
	float velocity = m_Speed * deltaTime;

	switch (direction) {
		case FORWARD: m_Pos += m_Front * velocity; break;
		case BACKWARDS: m_Pos -= m_Front * velocity; break;
		case LEFT: m_Pos -= m_Right * velocity; break;
		case RIGHT: m_Pos += m_Right * velocity; break;
		default: break;
	}
}

void Camera::updateRotation(float xoff, float yoff) {
	xoff *= m_Sensitivity;
	yoff *= m_Sensitivity;

	m_Yaw += xoff;
	m_Pitch += yoff;

	if (m_Pitch > 89.f) m_Pitch = 89.f;
	else if (m_Pitch < -89.f) m_Pitch = -89.f;

	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 f = glm::vec3(
		cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
		sin(glm::radians(m_Pitch)),
		sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));

	m_Front = glm::normalize(f);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

