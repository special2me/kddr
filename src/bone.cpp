#include "bone.h"

void Bone::update(float t) {
	glm::mat4 translation = interpolatePosition(0);
	glm::mat4 rotation = interpolateRotation(t);
	glm::mat4 scale = glm::mat4(1.f);

	m_LocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::interpolatePosition(float t) {
	return glm::translate(glm::mat4(1.f), m_Positions[(size_t)t].pos);
}

glm::mat4 Bone::interpolateRotation(float t) {
	return glm::toMat4(glm::normalize(m_Rotations[(size_t)t].ori));
}

