#include "bezier_surface.h"
#include <rand_float.hpp>





glm::vec3 eval_BezierCurve(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) noexcept
{
	float b0 = (1 - t) * (1 - t) * (1 - t);
	float b1 = 3 * t * (1 - t) * (1 - t);
	float b2 = 3 * t * t * (1 - t);
	float b3 = t * t * t;
	return p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
}


glm::vec3 eval_BezierSurface(const BezierSurface& s, float u, float v)
{
	ControlVertices pu;
	for (auto i = 0; i != 4; i++)
		pu[i] = eval_BezierCurve(s[i][0], s[i][1], s[i][2], s[i][3], u);

	return eval_BezierCurve(pu[0], pu[1], pu[2], pu[3], v);
}

glm::vec3 calc_rand_uv(unsigned int i, unsigned int j, float h)
{

	constexpr float div = 0.25;
	float u_min = (float)j * div;
	float u_max = ((float)j + 1.0f) * div;

	float v_min = (float)i * div;
	float v_max = ((float)i + 1.0f) * div;


	static RNG_float rng;
	return { rng(u_min, u_max), rng(v_min, v_max), h };
}

BezierSurface gen_BezierSurfaceMask(float outer_h, float inner_h) noexcept
{
	BezierSurface mask;


	for (unsigned int i = 0; i != 4; i++)
		for (unsigned int j = 0; j != 4; j++)
		{
			if (i == 0 || i == 3)
				mask[i][j] = calc_rand_uv(i, j, outer_h);

			if (i == 1 || i == 2)
			{
				if (j == 0 || j == 3)
					mask[i][j] = calc_rand_uv(i, j, outer_h);
				else
					mask[i][j] = calc_rand_uv(i, j, inner_h);
			}
		}
	return mask;
}



ControlVertexIndex get_BSurfaceCVI(int e_i, int edge_offset, int i) noexcept
{
	if (e_i == 0)
		return { e_i + edge_offset, i };

	if (e_i == 1)
		return { i, 3 - edge_offset };

	if (e_i == 2)
		return { 3 - edge_offset, i };

	if (e_i == 3)
		return { i, edge_offset };


	ControlVertexIndex v{ -1, -1 };
	return v;
}