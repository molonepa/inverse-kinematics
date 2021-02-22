#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<unsigned int> indices) {
	IndexedModel indexedModel;

	for (std::size_t i = 0; i < positions.size(); i++) {
		indexedModel.positions.push_back(positions[i]);
		indexedModel.uvs.push_back(uvs[i]);
		indexedModel.normals.push_back(normals[i]);
	}

	for (std::size_t i = 0; i < indices.size(); i++) {
		indexedModel.indices.push_back(indices[i]);
	}

	GenerateVertexBuffers(indexedModel);
}

Mesh::Mesh(const std::string& filename) {
	OBJModel objModel = OBJModel(filename);
	IndexedModel indexedModel = objModel.ToIndexedModel();

	GenerateVertexBuffers(indexedModel);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Render(GLenum mode) {
	glBindVertexArray(m_VAO);
	glDrawElements(mode, m_DrawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::GenerateVertexBuffers(const IndexedModel& indexedModel) {
	m_DrawCount = indexedModel.indices.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(m_NUM_BUFFERS, m_VBO);

	// push vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[m_POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, indexedModel.positions.size() * sizeof(indexedModel.positions[0]), &indexedModel.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// push vertex uvs
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[m_UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, indexedModel.uvs.size() * sizeof(indexedModel.uvs[0]), &indexedModel.uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// push vertex normals
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[m_NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, indexedModel.normals.size() * sizeof(indexedModel.normals[0]), &indexedModel.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// push indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[m_INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexedModel.indices.size() * sizeof(indexedModel.indices[0]), &indexedModel.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}