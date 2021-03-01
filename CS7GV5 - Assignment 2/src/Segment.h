#pragma once

#include <glm/glm.hpp>

class Segment {
public:
	Segment(glm::vec3 position) {
		m_Position = position;
	}

	inline glm::vec3 GetPosition() { return m_Position; }
	inline void SetPosition(glm::vec3 position) { m_Position = position; }

private:
	glm::vec3 m_Position;
};
