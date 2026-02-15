#include "animation.h"

bool Animation::parse(const std::string &path, Importer &imp) {
	SceneData scene;
	if (!imp.loadMesh(path, &scene)) return false;

	std::map<std::string, std::vector<int>> parentChildren;

	m_Duration = 0.f;
	for (size_t i = 0; i < scene.bones.size(); i++) {
		m_Bones.push_back(Bone(i, scene.bones[i]));
		
		float d = m_Bones.back().getDuration();
		if (m_Duration < d) m_Duration = d;

		parentChildren[scene.bones[i].parentName].push_back(i);
	}

	m_Root.bone = nullptr;
	return parseChildren(&m_Root, parentChildren);
}

bool Animation::parseChildren(AnimationNode *node, std::map<std::string, std::vector<int>> &parentChildren) {
	std::string parentName = "";
	if (node->bone) parentName = node->bone->getName();

	auto search = parentChildren.find(parentName);
	if (search == parentChildren.end()) return true;

	for (const int &child : parentChildren[parentName]) {
		node->children.emplace_back();
		node->children.back().bone = &m_Bones[child];
		parseChildren(&(node->children.back()), parentChildren);
	}

	return true;
}

