#include "bone.h"

void Bone::update(float t) {
	glm::mat4 translation = interpolatePosition(t);
	glm::mat4 rotation = interpolateRotation(t);
	glm::mat4 scale = glm::mat4(1.f);

	m_LocalTransform = translation * rotation * scale;
}

float Bone::getScaleFactor(float t0, float t1, float t) {
	return (t - t0) / (t1 - t0);
}

glm::mat4 Bone::interpolatePosition(float t) {
	if (m_Positions.size() == 0) {
		return glm::mat4(1.f);
	} else if (m_Positions.size() == 1) {
		return glm::translate(glm::mat4(1.f), m_Positions[0].pos);
	} else {
		int search = -1;

		for (size_t i = 1; i < m_Positions.size() && search < 0; i++) {
			float t0 = m_Positions[i - 1].time;	
			float t1 = m_Positions[i].time;

			if (t > t0 && t <= t1) search = i;
		}

		float scale = getScaleFactor(m_Positions[search - 1].time, m_Positions[search].time, t);
		return glm::translate(glm::mat4(1.f), glm::mix(m_Positions[search - 1].pos, m_Positions[search].pos, scale));
	}
}

glm::mat4 Bone::interpolateRotation(float t) {
	if (m_Rotations.size() == 0) {
		return glm::mat4(1.f);
	} else if (m_Rotations.size() == 1) {
		return glm::toMat4(glm::normalize(m_Rotations[0].ori));
	} else {
		int search = -1;

		for (size_t i = 1; i < m_Rotations.size() && search < 0; i++) {
			float t0 = m_Rotations[i - 1].time;
			float t1 = m_Rotations[i].time;

			if (t > t0 && t <= t1) search = i;
		}

		float scale = getScaleFactor(m_Rotations[search - 1].time, m_Rotations[search].time, t);
		return glm::toMat4(glm::normalize(glm::slerp(m_Rotations[search - 1].ori, m_Rotations[search].ori, scale)));
	}
}

