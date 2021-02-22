#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform {
public:
	Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f)) {
		this->m_Position = position;
		this->m_Rotation = rotation;
		this->m_Scale = scale;
	}

	inline glm::vec3 GetPosition() { return m_Position; }
	inline glm::vec3 GetRotation() { return m_Rotation; }
	inline glm::vec3 GetScale() { return m_Scale; }

	inline void SetPosition(glm::vec3 p) { m_Position = p; }
	inline void SetRotation(glm::vec3 r) { m_Rotation = r; }
	inline void SetScale(glm::vec3 s) { m_Scale = s; }

	inline glm::mat4 GetModelMatrix() const {
		glm::mat4 posMatrix = glm::translate(m_Position);

		glm::mat4 rotXMatrix = glm::rotate(m_Rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(m_Rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(m_Rotation.z, glm::vec3(0, 0, 1));
		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		glm::mat4 scaleMatrix = glm::scale(m_Scale);

		return posMatrix * rotMatrix * scaleMatrix;
	}

private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
};
