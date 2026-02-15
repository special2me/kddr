#include "animator.h"

void Animator::update(float dt) {
	if (m_CurrentAnimation) {
		m_CurrentTime += m_CurrentAnimation->getFPS() * dt;

		float d = m_CurrentAnimation->getDuration();
		m_CurrentTime = fmod(m_CurrentTime, d);

		calculateBoneTransform(m_CurrentAnimation->getRoot(), glm::mat4(1.f));
	}
}

void Animator::calculateBoneTransform(const AnimationNode *node, glm::mat4 parentTransform) {
	Bone *bone = node->bone;	
	glm::mat4 globalTransform = glm::mat4(1.f);
	globalTransform = glm::rotate(globalTransform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	if (bone) {
		bone->update(m_CurrentTime);

		glm::mat4 nodeTransform = bone->getLocalTransform();
		globalTransform = parentTransform * nodeTransform;

		m_BoneMatricies[bone->getID()] = globalTransform;
	}

	for (size_t i = 0; i < node->children.size(); i++) {
		calculateBoneTransform(&node->children[i], globalTransform);
	}
}

