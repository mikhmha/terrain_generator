#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>


using ControlVertices = std::array<glm::vec3, 4>;
using BezierSurface = std::array<ControlVertices, 4>;




glm::vec3 eval_BezierCurve(const glm::vec3 &p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) noexcept;
glm::vec3 eval_BezierSurface(const BezierSurface& s, float u, float v);		// implicit equation for a Bezier Surface

BezierSurface gen_BezierSurfaceMask(float outer_h, float inner_h) noexcept;
glm::vec3 calc_rand_uv(unsigned int i, unsigned int j, float h);

using ControlVertexIndex = std::array<int, 2>;
ControlVertexIndex get_BSurfaceCVI(int e_i, int edge_offset, int i) noexcept;

