#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "logger.h"

class Shader {
	public:
		Shader(const std::string &vsPath, const std::string &fsPath) {
			m_Ready = loadShader(vsPath, fsPath);		
		}

		bool is_ready() { 
			if (!m_Ready) LOG_DEBUG("WARNING! Attemping action on string when not ready");
			return m_Ready;
		}

		void use() { 
			if (is_ready()) glUseProgram(m_ID); 
		}

		void close() {
			if (is_ready()) glDeleteProgram(m_ID); m_Ready = false;
		}

		void setVec4(const std::string &name, const glm::vec4 &vec) {
			if (is_ready()) glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
		}

		void setMat4(const std::string &name, const glm::mat4 &mat) {
			if (is_ready()) glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

	private:
		bool m_Ready;
		GLuint m_ID;

		bool loadShader(const std::string &vsPath, const std::string &fsPath);
};

#endif

