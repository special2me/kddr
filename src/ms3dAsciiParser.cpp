#include "ms3dAsciiParser.h"

#define S_MESH scene->meshes.back()
#define S_VERTEX S_MESH.vertices.back()
#define S_NORMAL S_MESH.normals.back()
#define S_FACE S_MESH.faces.back()
#define S_MAT scene->materials.back()
#define S_BONE scene->bones.back()
#define S_POSKEY S_BONE.posKeys.back()
#define S_ROTKEY S_BONE.rotKeys.back()

Ms3dAsciiParser::Ms3dAsciiParser() {

}

Ms3dAsciiParser::~Ms3dAsciiParser() {

}

void nextParameter(std::string &line) {
	if (!line.empty()) {
		std::size_t f = line.find(" ");
		if (f == std::string::npos) line = "";
		else line = line.substr(f + 1);
	}
}

template <typename T> bool readParameter(std::string &line, T * data, int nParams = 1) {
	std::istringstream iss(line);
	for (int i = 0; i < nParams; i++) {
		if (!(iss >> *data)) {
			LOG_ERROR("Invalid parameter " << i << " at line -- " << line);
			return false;
		}

		data++;
		nextParameter(line);
	}

	return true;
}

bool readParameterS(std::string &line, std::string * data, int nParams = 1) {
	for (int i = 0; i < nParams; i++) {
		if (!readParameter<std::string>(line, data, 1)) return false;

		if ((*data)[0] == '"') {
			*data = data->substr(1);

			while ((*data)[data->size() - 1] != '"') {
				std::string dp;
				if (!readParameter<std::string>(line, &dp, 1)) return false;
				*data = *data + " " + dp;
			}

			*data = data->substr(0, data->size() - 1);
		}
	}

	return true;
}

bool Ms3dAsciiParser::loadMesh(const std::string &path, SceneData *scene) {
	std::ifstream mDataFile;
	std::stringstream mDataBuf;
	std::string mData;

	mDataFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		mDataFile.open(path);
		mDataBuf << mDataFile.rdbuf();
		mDataFile.close();
	} catch (std::ifstream::failure &e) {
		LOG_ERROR("Failed to open file " << path << " -- " << e.what());
		return false;
	}

	ParserState ps = FIND_PROPERTY;

	int meshCount, meshTotal;
	int vertexCount, meshVertexTotal;
	int normalCount, meshNormalTotal;
	int triangleCount, meshTriangleTotal;
	int materialCount, materialTotal;
	int boneCount, boneTotal;
	int bonePositionKeysCount, bonePositionKeysTotal;
	int boneRotationKeysCount, boneRotationKeysTotal;

	std::string label;

	LOG_DEBUG("Parsing ms3dascii model " << path);

	for (std::string line; std::getline(mDataBuf, line);) {
		// remove whitespace
		while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
			line = line.substr(1);

		while (!line.empty() && (line[line.size() - 1] == ' ' || line[line.size() - 1] == '\t' || line[line.size() - 1] == '\r'))
			line = line.substr(0, line.size() - 1);

		// check for empty/comment line
		if (line.empty() || line[0] == '\r' || (line[0] == '/' && line[1] == '/')) continue;

		switch (ps) {
			case FIND_PROPERTY:
				if (!readParameterS(line, &label)) return false;

				if (label == "Frames:") {
					if (!readParameter<int>(line, &scene->frameTotal)) return false;

					ps = FIND_PROPERTY;
				} else if (label == "Frame:") {
					if (!readParameter<int>(line, &scene->frame)) return false;

					ps = FIND_PROPERTY;
				} else if (label == "Meshes:") {
					if (!readParameter<int>(line, &meshTotal)) return false;
					meshCount = 0;

					ps = MESH_DATA;
				} else if (label == "Materials:") {
					if (!readParameter<int>(line, &materialTotal)) return false;
					materialCount = 0;

					ps = MATERIAL_NAME;
				} else if (label == "Bones:") {
					if (!readParameter<int>(line, &boneTotal)) return false;
					boneCount = 0;

					ps = BONE_NAME;
				} else {
					LOG_ERROR("Invalid property name \"" << label << "\" at line -- " << line);
					return false;
				}

				break;

			case MESH_DATA:
				scene->meshes.emplace_back();

				if (!readParameterS(line, &(S_MESH.name))) return false;
				nextParameter(line);
				if (!readParameter<int>(line, &(S_MESH.materialID))) return false;

				meshCount++;
				ps = MESH_VERTEXTOTAL;
				break;	

			case MESH_VERTEXTOTAL:
				if (!readParameter<int>(line, &meshVertexTotal)) return false;

				vertexCount = 0;
				ps = MESH_VERTEX;
				break;

			case MESH_VERTEX:
				S_MESH.vertices.emplace_back();

				nextParameter(line);
				if (!readParameter<float>(line, &(S_VERTEX.pos[0]), 3)) return false;
				if (!readParameter<float>(line, &(S_VERTEX.tex[0]), 2)) return false;
				if (!readParameter<int>(line, &(S_VERTEX.boneID))) return false;

				vertexCount++;

				if (vertexCount >= meshVertexTotal) {
					ps = MESH_NORMALTOTAL;
				}

				break;
			
			case MESH_NORMALTOTAL:
				if (!readParameter<int>(line, &meshNormalTotal)) return false;

				normalCount = 0;
				ps = MESH_NORMAL;
				break;

			case MESH_NORMAL:
				S_MESH.normals.emplace_back();

				if (!readParameter<float>(line, &(S_MESH.normals.back().dir[0]), 3)) return false;

				normalCount++;

				if (normalCount >= meshNormalTotal) {
					ps = MESH_TRIANGLETOTAL;
				}
				break;

			case MESH_TRIANGLETOTAL:
				if (!readParameter<int>(line, &meshTriangleTotal)) return false;

				triangleCount = 0;
				ps = MESH_TRIANGLE;
				break;

			case MESH_TRIANGLE:
				S_MESH.faces.emplace_back();
				S_FACE.nEdges = 3; // triangle

				nextParameter(line);
				if (!readParameter<int>(line, &(S_FACE.vertexIDs[0]), 3)) return false;
				if (!readParameter<int>(line, &(S_FACE.normalIDs[0]), 3)) return false;
				if (!readParameter<int>(line, &(S_FACE.smoothingGroup))) return false;

				triangleCount++;

				if (triangleCount >= meshTriangleTotal) {
					if (meshCount >= meshTotal) {
						LOG_DEBUG("\tFinished parsing " << scene->meshes.size() << " mesh(es)");
						ps = FIND_PROPERTY;
					} else {
						ps = MESH_DATA;
					}
				}
				break;

			case MATERIAL_NAME:
				scene->materials.emplace_back();
				if (!readParameterS(line, &(S_MAT.name))) return false;

				materialCount++;
				ps = MATERIAL_AMBIENT;
				break;

			case MATERIAL_AMBIENT:
				if (!readParameter<float>(line, &(S_MAT.ambient[0]), 4)) return false;
				ps = MATERIAL_DIFFUSE;
				break;

			case MATERIAL_DIFFUSE:
				if (!readParameter<float>(line, &(S_MAT.diffuse[0]), 4)) return false;
				ps = MATERIAL_SPECULAR;
				break;

			case MATERIAL_SPECULAR:
				if (!readParameter<float>(line, &(S_MAT.specular[0]), 4)) return false;
				ps = MATERIAL_EMISSIVE;
				break;

			case MATERIAL_EMISSIVE:
				if (!readParameter<float>(line, &(S_MAT.emissive[0]), 4)) return false;
				ps = MATERIAL_SHININESS;
				break;

			case MATERIAL_SHININESS:
				if (!readParameter<float>(line, &(S_MAT.shininess))) return false;
				ps = MATERIAL_TRANSPARENCY;
				break;

			case MATERIAL_TRANSPARENCY:
				if (!readParameter<float>(line, &(S_MAT.transparency))) return false;
				ps = MATERIAL_COLORMAP;
				break;

			case MATERIAL_COLORMAP:
				if (!readParameterS(line, &(S_MAT.colorMap))) return false;
				ps = MATERIAL_ALPHAMAP;
				break;

			case MATERIAL_ALPHAMAP:
				if (!readParameterS(line, &(S_MAT.alphaMap))) return false;

				if (materialCount >= materialTotal) {
					LOG_DEBUG("\tFinished parsing " << scene->materials.size() << " material(s)");
					ps = FIND_PROPERTY;
				} else {
					ps = MATERIAL_NAME;
				}
				break;

			case BONE_NAME:
				scene->bones.emplace_back();
				if (!readParameterS(line, &(S_BONE.name))) return false;
				
				boneCount++;
				ps = BONE_PARENT;
				break;

			case BONE_PARENT:
				if (!readParameterS(line, &(S_BONE.parentName))) return false;
				ps = BONE_DATA;
				break;

			case BONE_DATA:
				nextParameter(line);
				if (!readParameter<float>(line, &(S_BONE.pos[0]), 3)) return false;
				if (!readParameter<float>(line, &(S_BONE.rot[0]), 3)) return false;
				ps = BONE_POSITIONKEYS_TOTAL;
				break;

			case BONE_POSITIONKEYS_TOTAL:
				if (!readParameter<int>(line, &bonePositionKeysTotal)) return false;

				bonePositionKeysCount = 0;
				ps = BONE_POSITIONKEYS_DATA;
				break;

			case BONE_POSITIONKEYS_DATA:
				S_BONE.posKeys.emplace_back();
				if (!readParameter<float>(line, &(S_POSKEY.time))) return false;
				if (!readParameter<float>(line, &(S_POSKEY.data[0]), 3)) return false;

				bonePositionKeysCount++;
				if (bonePositionKeysCount >= bonePositionKeysTotal) {
					ps = BONE_ROTATIONKEYS_TOTAL;
				}
				break;

			case BONE_ROTATIONKEYS_TOTAL:
				if (!readParameter<int>(line, &boneRotationKeysTotal)) return false;

				boneRotationKeysCount = 0;
				ps = BONE_ROTATIONKEYS_DATA;
				break;

			case BONE_ROTATIONKEYS_DATA:
				S_BONE.rotKeys.emplace_back();
				if (!readParameter<float>(line, &(S_ROTKEY.time))) return false;
				if (!readParameter<float>(line, &(S_ROTKEY.data[0]), 3)) return false;

				boneRotationKeysCount++;
				if (boneRotationKeysCount >= boneRotationKeysTotal) {
					if (boneCount >= boneTotal) {
						LOG_DEBUG("\tFinished parsing " << scene->bones.size() << " bone(s)");
						ps = COMPLETE;
					} else {
						ps = BONE_NAME;
					}
				}
				break;

			case COMPLETE:
				LOG_DEBUG("Finished parsing ms3dascii model " << path);
				ps = NOP;
				break;

			case NOP:
				continue;

			default:
				LOG_ERROR("Unexpected parser state -- " << ps);
				return false;
		}
	}

	return true;
}

