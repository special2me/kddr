#ifndef MESH_H_
#define MESH_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "logger.h"
#include "importer.h"
#include "shader.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
	GLuint boneID;
};

struct Material {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissive;
	GLfloat shininess;
	GLfloat transparency;
};

class Mesh {
	public:
		Mesh(const MeshData &mesh, const MaterialData &mat) {
			std::vector<std::pair<int, int>> vnPairs;

			for (const FaceData &face : mesh.faces) {
				for (int i = 0; i < 3; i++) {
					int vID = face.vertexIDs[i];
					int nID = face.normalIDs[i];

					int id = -1;
					for (size_t search = 0; search < vnPairs.size() && id < 0; search++) {
						if (vnPairs[search].first == vID && vnPairs[search].second == nID) {
							id = search;
						}
					}

					if (id < 0) {
						vnPairs.push_back(std::pair<int, int>(vID, nID));

						VertexData v = mesh.vertices[vID];
						NormalData n = mesh.normals[nID];

						m_Vertices.emplace_back();
						m_Vertices.back().pos = glm::vec3(v.pos[0], v.pos[1], v.pos[2]);
						m_Vertices.back().nor = glm::vec3(n.dir[0], n.dir[1], n.dir[2]);
						m_Vertices.back().tex = glm::vec2(v.tex[0], v.tex[1]);
						m_Vertices.back().boneID = v.boneID;

						
						id = m_Vertices.size() - 1;
					}

					m_Indices.push_back(id);
				}
			}

			m_Material.ambient = glm::vec4(mat.ambient[0], mat.ambient[1], mat.ambient[2], mat.ambient[3]);
			m_Material.diffuse = glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], mat.diffuse[3]);
			m_Material.specular = glm::vec4(mat.specular[0], mat.specular[1], mat.specular[2], mat.specular[3]);
			m_Material.emissive = glm::vec4(mat.emissive[0], mat.emissive[1], mat.emissive[2], mat.emissive[3]);
			m_Material.shininess = mat.shininess;
			m_Material.transparency = mat.transparency;

			generateMeshVAO();
		}

		void draw(Shader &shader);

	private:
		GLuint VAO, VBO, EBO;

		std::vector<Vertex> m_Vertices;
		std::vector<GLuint> m_Indices;
		Material m_Material;

		void generateMeshVAO();
};

#endif

