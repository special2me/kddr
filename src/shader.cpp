#include "shader.h"

bool Shader::loadShader(const std::string &vsPath, const std::string &fsPath) {
	std::ifstream vsFile, fsFile;
	std::stringstream vsBuf, fsBuf;
	std::string vsCode, fsCode;

	try {
		vsFile.open(vsPath);
		fsFile.open(fsPath);

		vsBuf << vsFile.rdbuf();
		vsCode = vsBuf.str();

		fsBuf << fsFile.rdbuf();
		fsCode = fsBuf.str();

		vsFile.close();
		fsFile.close();
	} catch (std::ifstream::failure &e) {
		LOG_ERROR("Shader not successfully read -- " << e.what());
		return false;
	}

	GLuint vsID, fsID;
	const char* vsCodeData = vsCode.c_str();
	const char* fsCodeData = fsCode.c_str();

	GLint compileSuccess;
	GLchar compileInfoLog[1024];

	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vsCodeData, NULL);
	glCompileShader(vsID);

	glGetShaderiv(vsID, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		glGetShaderInfoLog(vsID, 1024, NULL, compileInfoLog);
		LOG_ERROR("Vertex shader failed to compile -- " << compileInfoLog);
		return false;
	}

	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &fsCodeData, NULL);
	glCompileShader(fsID);

	glGetShaderiv(fsID, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		glGetShaderInfoLog(fsID, 1024, NULL, compileInfoLog);
		LOG_ERROR("Fragment shader failed to compile -- " << compileInfoLog);
		return false;
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vsID);
	glAttachShader(m_ID, fsID);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &compileSuccess);
	if (!compileSuccess) {
		glGetProgramInfoLog(m_ID, 1024, NULL, compileInfoLog);
		LOG_ERROR("Shader failed to link -- " << compileInfoLog);
		return false;
	}

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return true;
}


