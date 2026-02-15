#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <glm/glm.hpp>

#include <vector>
#include <map>

#include "logger.h"
#include "parsable.h"
#include "importer.h"
#include "bone.h"

struct AnimationNode {
	Bone* bone;
	std::vector<AnimationNode> children;
};

class Animation : public Parsable {
	public:
		Animation(const std::string &path, Importer &imp) { 
			m_FPS = 30.f;
			m_Duration = 0.f;

			readFile(path, imp);
		}

		const AnimationNode *getRoot() { return &m_Root; }
		float getFPS() { return m_FPS; }
		float getDuration() { return m_Duration; }

	private:
		std::vector<Bone> m_Bones;
		AnimationNode m_Root;

		float m_FPS;
		float m_Duration;
		
		bool parse(const std::string &path, Importer &imp);
		bool parseChildren(AnimationNode *node, std::map<std::string, std::vector<int>> &parentChildren);
};

#endif

