#ifndef BONE_H_
#define BONE_H_

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

#include "logger.h"
#include "importer.h"

struct KeyPosition {
	float time;
	glm::vec3 pos;
};

struct KeyRotation {
	float time;
	glm::quat ori;
};

class Bone {
	public:
		Bone(size_t id, const BoneData &data) {
			m_ID = id;
			m_Name = data.name;
			m_ParentName = data.parentName;

			m_BasePos = glm::vec3(data.pos[0], data.pos[1], data.pos[2]);
			m_BaseRot = glm::vec3(data.rot[0], data.rot[1], data.rot[2]);

			m_Duration = 0.f;

			for (const KeyData &key : data.posKeys) {
				m_Positions.emplace_back();
				m_Positions.back().time = key.time;
				m_Positions.back().pos = glm::vec3(key.data[0], key.data[1], key.data[2]);

				if (key.time > m_Duration) m_Duration = key.time;
			}

			for (const KeyData &key : data.rotKeys) {
				m_Rotations.emplace_back();
				m_Rotations.back().time = key.time;
				m_Rotations.back().ori = glm::quat(glm::vec3(key.data[0], key.data[1], key.data[2]));

				if (key.time > m_Duration) m_Duration = key.time;
			}

			m_LocalTransform = glm::mat4(1.f);
		}

		size_t getID() { return m_ID; }
		const std::string &getName() { return m_Name; }
		float getDuration() { return m_Duration; }
		const glm::mat4 &getLocalTransform() { return m_LocalTransform; }

		void update(float t);

	private:
		size_t m_ID;

		std::string m_Name;
		std::string m_ParentName;

		glm::vec3 m_BasePos;
		glm::vec3 m_BaseRot;

		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;

		float m_Duration;
		glm::mat4 m_LocalTransform;

		glm::mat4 interpolatePosition(float t);
		glm::mat4 interpolateRotation(float t);

		float getScaleFactor(float t0, float t1, float t);
};

#endif

