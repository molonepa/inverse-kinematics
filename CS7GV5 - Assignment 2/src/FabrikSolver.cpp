#include "FabrikSolver.h"
#include <iostream>

#define MAX_ITERATIONS 100

float euclideanDistance(glm::vec3 a, glm::vec3 b) {
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;
	return sqrt((x * x) + (y * y) + (z * z));
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float v) {
	float x = ((1 - v) * a.x) + (v * b.x);
	float y = ((1 - v) * a.y) + (v * b.y);
	float z = ((1 - v) * a.z) + (v * b.z);
	return glm::vec3(x, y, z);
}

// https://stackoverflow.com/questions/22087193/optimized-functions-to-compute-projection-of-a-point-on-a-line
glm::vec3 projection(glm::vec3 p, glm::vec3 l) {
	float m2 = glm::dot(p, p);
	return l * (glm::dot(l, p) / m2);
}

FabrikSolver::FabrikSolver(std::vector<Joint> chain, float tolerance) {
	m_Chain = chain;
	std::cout << chain.size() << std::endl;

	for (size_t i = 0; i < chain.size() - 1; i++) {
		float length = euclideanDistance(chain[i].GetPosition(), chain[i + 1].GetPosition());
		m_Lengths.push_back(length);
		m_TotalLength += length;
	}

	m_Tolerance = tolerance;
}

void FabrikSolver::ReachForwards(glm::vec3 target) {
	m_Chain.back().SetPosition(target);
	for (size_t i = m_Chain.size() - 2; i > 0; i--) {
		float lambda = m_Lengths[i] / euclideanDistance(m_Chain[i].GetPosition(), m_Chain[i + 1].GetPosition());
		glm::vec3 newPosition = lerp(m_Chain[i + 1].GetPosition(), m_Chain[i].GetPosition(), lambda);
		newPosition.y = glm::max(0.0f, newPosition.y);
		m_Chain[i].SetPosition(newPosition);
	}
}

void FabrikSolver::ReachBackwards(glm::vec3 target) {
	for (size_t i = 0; i < m_Chain.size() - 1; i++) {
		float lambda = m_Lengths[i] / euclideanDistance(m_Chain[i].GetPosition(), m_Chain[i + 1].GetPosition());
		glm::vec3 newPosition = lerp(m_Chain[i].GetPosition(), m_Chain[i + 1].GetPosition(), lambda);
		newPosition.y = glm::max(0.0f, newPosition.y);
		m_Chain[i + 1].SetPosition(newPosition);
	}
}

std::vector<Joint> FabrikSolver::Solve(glm::vec3 target) {
	// if target is out of reach
	if (euclideanDistance(m_Chain.front().GetPosition(), target) >= m_TotalLength) {
		for (size_t i = 0; i < m_Chain.size() - 1; i++) {
			float lambda = m_Lengths[i] / euclideanDistance(m_Chain[i].GetPosition(), target);
			glm::vec3 newPosition = lerp(m_Chain[i].GetPosition(), target, lambda);
			newPosition.y = glm::max(0.0f, newPosition.y);
			m_Chain[i + 1].SetPosition(newPosition);
		}
	}
	// if target is within reach
	else {
		for (int i = 0; i < MAX_ITERATIONS; i++) {
			if (euclideanDistance(m_Chain.back().GetPosition(), target) < m_Tolerance) {
				break;
			}
			ReachForwards(target);
			ReachBackwards(target);
		}
	}
	return m_Chain;
}

// wasn't able to implement rotational constraints in time

/*glm::vec3 FabrikSolver::ConstrainRotation(glm::vec3 p0, glm::vec3 p1, glm::vec3 target, glm::vec4 constraints) {
	// determine line passing through current and previous joint
	glm::vec3 L1 = glm::normalize(p1 - p0);
	// project target onto L1
	glm::vec3 O = projection(target, L1);
	// determine distance between O and current joint
	float S = euclideanDistance(O, p0);

	// rotation of L1 onto global y axis (such that constraint plane aligned with xz plane) https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
	glm::vec3 v = glm::cross(L1, glm::vec3(0.0f, 1.0f, 0.0f));
	float sine = glm::length(v);
	float cosine = glm::dot(L1, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat3 v_x = glm::mat3(0.0f, -v.z, v.y, v.z, 0.0f, -v.x, -v.y, v.x, 0.0f); // skew-symmetric cross product matrix of v
	glm::mat4 R = glm::mat4(1.0f); // identity matrix
	R += v_x + (v_x * v_x);
	R /= 1 + cosine;
	glm::vec3 t = (R * glm::vec4(target, 1.0)).xyz;
	// translation to origin
	t.y -= S;
	
	// calculate distances on constraint plane
	glm::vec4 q = glm::vec4(0.0f);
	q[0] = S * tan(constraints[0]);
	q[1] = S * tan(constraints[1]);
	q[2] = S * tan(constraints[2]);
	q[3] = S * tan(constraints[3]);

	// determine which quadrant t is in
	float quadrant = 0.0f;
	//if (t.x < 0 && t.x > q[0])
}*/
