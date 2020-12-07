#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>







glm::vec3 calc_triangle_normal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
glm::vec3 calc_point_on_line(const glm::vec3& p0, const glm::vec3& p1, float t);
glm::vec3 geometric_centre(const std::vector<glm::vec3>& v);
std::array<unsigned int, 2> find_minmax_texcoord_indices(const std::vector<glm::vec3>& v, const glm::vec3& dir, const glm::vec3& origin);


struct vec3Hash
{
	std::size_t operator()(const glm::vec3& v) const
	{
		return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z);
	}


	bool operator()(const glm::vec3& a, const glm::vec3& b) const
	{
		auto x = abs(a.x - b.x);
		auto y = abs(a.y - b.y);
		auto z = abs(a.z - b.z);

		return (x < 0.00005f) && (y < 0.00005) && (z < 0.00005f);
	}

};