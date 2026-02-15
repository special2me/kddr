#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include <glm/glm.hpp>

#include "logger.h"
#include "animation.h"

class Animator {
	public:
		Animator(Animation *animation) {
			m_CurrentAnimation = animation;
			
			m_BoneMatricies.reserve(MAX_BONES);
			for (size_t i = 0; i < m_BoneMatricies.capacity(); i++) {
				m_BoneMatricies.push_back(glm::mat4(1.f));
			}

			m_CurrentTime = 0.f;
		}

		const static int MAX_BONES = 100;

		const std::vector<glm::mat4> &getBoneMatricies() { return m_BoneMatricies; }

		void update(float dt);

	private:
		void calculateBoneTransform(const AnimationNode *node, glm::mat4 parentTransform);

		std::vector<glm::mat4> m_BoneMatricies;
		Animation* m_CurrentAnimation;

		float m_CurrentTime;
};

#endif

