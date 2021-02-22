#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
	Camera(const glm::vec3& position = glm::vec3(0.0f), float fov = 70.0f, float aspect = 1.77777f, float zNear = 0.1f, float zFar = 1000.0f, float sensitivity = 0.4f) {
		m_Position = position;
		m_Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Yaw = -90.0f;
		m_Pitch = 0.0f;
		m_Sensitivity = sensitivity;
		m_ZNear = zNear;
		m_ZFar = zFar;
		m_Projection = glm::perspective(fov, aspect, zNear, zFar);
	}

	inline glm::mat4 GetProjectionMatrix() {
		return m_Projection;
	}

	inline glm::mat4 GetViewMatrix() {
		return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	inline glm::vec3 GetPosition() { return m_Position; }

	inline void Look(int deltaX, int deltaY) {
		glm::vec3 front;

		m_Yaw += deltaX * m_Sensitivity;
		m_Pitch -= deltaY * m_Sensitivity;

		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}

		front.x = std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
		front.y = std::sin(glm::radians(m_Pitch));
		front.z = std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));

		m_Forward = glm::normalize(front);
	}

	inline void MoveForwards() {
		m_Position += 0.25f * m_Forward;
	}

	inline void MoveBackwards() {
		m_Position -= 0.25f * m_Forward;
	}

	inline void MoveRight() {
		m_Position += 0.25f * cross(m_Forward, m_Up);
	}

	inline void MoveLeft() {
		m_Position -= 0.25f * cross(m_Forward, m_Up);
	}

private:
	glm::vec3 m_Position;

	glm::vec3 m_Forward;
	glm::vec3 m_Up;

	glm::mat4 m_Projection;
	float m_ZNear;
	float m_ZFar;

	float m_Yaw;
	float m_Pitch;
	float m_Sensitivity;
};
