#include "model.h"

Model::Model(const std::string &path, Importer &imp) {
	m_Ready = loadModel(path, imp);
}

void Model::draw(Shader &shader) {
	for (Mesh &mesh : m_Meshes) {
		mesh.draw(shader);
	}
}

bool Model::loadModel(const std::string &path, Importer &imp) {
	SceneData scene;
	if (!imp.loadMesh(path, &scene)) return false;

	for (const MeshData &mesh : scene.meshes) {
		MaterialData mat = scene.materials[mesh.materialID];
		m_Meshes.push_back(Mesh(mesh, mat));
	}

	return true;
}

