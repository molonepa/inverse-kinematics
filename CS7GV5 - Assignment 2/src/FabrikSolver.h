#pragma once

#include <vector>
#include "Segment.h"

class FabrikSolver {
public:
	FabrikSolver(std::vector<Segment> chain, float tolerance = 0.00001);

	void ReachForwards(glm::vec3 target);
	void ReachBackwards(glm::vec3 target);
	std::vector<Segment> Solve(glm::vec3 target);

	inline std::vector<float> GetLengths() { return m_Lengths; }
	inline void SetLengths(std::vector<float> lengths) { m_Lengths = lengths; }

private:
	std::vector<Segment> m_Chain;
	std::vector<float> m_Lengths;
	float m_TotalLength;
	float m_Tolerance;
};

