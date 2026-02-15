#ifndef IMPORTER_H_
#define IMPORTER_H_

struct VertexData {
	float pos[3];
	float tex[2];
	int boneID;
};

struct NormalData {
	float dir[3];
};

struct FaceData {
	int nEdges;
	int vertexIDs[3];
	int normalIDs[3];
	int smoothingGroup;
};

struct MeshData {
	std::string name;
	int materialID;

	std::vector<VertexData> vertices;
	std::vector<NormalData> normals;
	std::vector<FaceData> faces;
};

struct MaterialData {
	std::string name;

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;
	float transparency;

	std::string colorMap;
	std::string alphaMap;
};

struct KeyData {
	float time;
	float data[3];
};

struct BoneData {
	std::string name;
	std::string parentName;

	float pos[3];
	float rot[3];
	std::vector<KeyData> posKeys;
	std::vector<KeyData> rotKeys;
};

struct SceneData {
	int frameTotal;
	int frame;

	std::vector<MeshData> meshes;
	std::vector<MaterialData> materials;
	std::vector<BoneData> bones;
};

class Importer {
	public:
		Importer() {

		}

		virtual bool loadMesh(const std::string &path, SceneData *scene) = 0;	
};

#endif

