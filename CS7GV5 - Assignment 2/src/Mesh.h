#pragma once

#include <vector>

#include <GL/glew.h>
#include <obj_loader/obj_loader.hpp>

class Mesh {
public:
	Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<unsigned int> indices);
	Mesh(const std::string& filename);
	virtual ~Mesh();

	void Render(GLenum mode = GL_TRIANGLES);

private:
	void GenerateVertexBuffers(const IndexedModel& indexedModel);

	enum {
		m_POSITION_VB,
		m_UV_VB,
		m_NORMAL_VB,
		m_INDEX_VB,
		m_NUM_BUFFERS
	};

	GLuint m_VAO;
	GLuint m_VBO[m_NUM_BUFFERS];

	unsigned int m_DrawCount;
};
