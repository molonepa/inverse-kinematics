#pragma once

#include <glm/glm.hpp>

class Joint {
public:
	Joint(glm::vec3 position, glm::vec4 constraints = glm::vec4(45.0f)) {
		m_Position = position;
		m_Constraints = constraints;
	}

	inline glm::vec3 GetPosition() { return m_Position; }
	inline void SetPosition(glm::vec3 position) { m_Position = position; }

	inline glm::vec4 GetConstraints() { return m_Constraints; }
	inline void SetConstraints(glm::vec4 constraints) { m_Constraints = constraints; }

private:
	glm::vec3 m_Position;
	glm::vec4 m_Constraints;
};
