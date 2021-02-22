#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
public:
	Shader(const std::string& filename);
	virtual ~Shader();

	void Bind();

	void UpdateUniform(std::string name, const bool u);
	void UpdateUniform(std::string name, const int u);
	void UpdateUniform(std::string name, const float u);
	void UpdateUniform(std::string name, const double u);
	void UpdateUniform(std::string name, const glm::vec2 u);
	void UpdateUniform(std::string name, const glm::vec3 u);
	void UpdateUniform(std::string name, const glm::vec4 u);
	void UpdateUniform(std::string name, const glm::mat3 u);
	void UpdateUniform(std::string name, const glm::mat4 u);

private:
	static const unsigned int msc_NumShaderPrograms = 2;

	GLuint m_shaderID;
	GLuint m_shaderPrograms[msc_NumShaderPrograms];
};