#ifndef SCENE_H_
#define SCENE_H_

#include <string>

#include "logger.h"
#include "importer.h"
#include "mesh.h"
#include "shader.h"

class Model {
	public:
		Model(const std::string &path, Importer &imp);

		bool is_ready() { return m_Ready; }
		void draw(Shader &shader);

	private:
		bool m_Ready;
		std::vector<Mesh> m_Meshes;
		// TODO: load textures here

		bool loadModel(const std::string &path, Importer &imp);
};

#endif

