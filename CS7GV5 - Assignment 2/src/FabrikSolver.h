#pragma once

#ifndef GLM_SWIZZLE
	#define GLM_SWIZZLE
#endif

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Joint.h"

class FabrikSolver {
public:
	FabrikSolver(std::vector<Joint> chain, float tolerance = 0.00001);

	std::vector<Joint> Solve(glm::vec3 target);

	inline std::vector<float> GetLengths() { return m_Lengths; }
	inline void SetLengths(std::vector<float> lengths) { m_Lengths = lengths; }


private:
	void ReachForwards(glm::vec3 target);
	void ReachBackwards(glm::vec3 target);
	//glm::vec3 ConstrainRotation(glm::vec3 basePosition, glm::vec3 endPosition, glm::vec3 target, glm::vec4 constraints);
	//glm::vec3 ConstrainOrientation();

	std::vector<Joint> m_Chain;
	std::vector<float> m_Lengths;
	float m_TotalLength;
	float m_Tolerance;
};

