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

FabrikSolver::FabrikSolver(std::vector<Segment> chain, float tolerance) {
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
		m_Chain[i].SetPosition(lerp(m_Chain[i + 1].GetPosition(), m_Chain[i].GetPosition(), lambda));
	}
}

void FabrikSolver::ReachBackwards(glm::vec3 target) {
	for (size_t i = 0; i < m_Chain.size() - 1; i++) {
		float lambda = m_Lengths[i] / euclideanDistance(m_Chain[i].GetPosition(), m_Chain[i + 1].GetPosition());
		m_Chain[i + 1].SetPosition(lerp(m_Chain[i].GetPosition(), m_Chain[i + 1].GetPosition(), lambda));
	}
}

std::vector<Segment> FabrikSolver::Solve(glm::vec3 target) {
	// if target is out of reach
	if (euclideanDistance(m_Chain.front().GetPosition(), target) >= m_TotalLength) {
		for (size_t i = 0; i < m_Chain.size() - 1; i++) {
			float lambda = m_Lengths[i] / euclideanDistance(m_Chain[i].GetPosition(), target);
			m_Chain[i + 1].SetPosition(lerp(m_Chain[i].GetPosition(), target, lambda));
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
