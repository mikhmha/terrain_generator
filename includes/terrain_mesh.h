#pragma once


#include <bezier_surface.h>
#include <texture.h>
class Shader;


class TerrainMesh {
public:
	TerrainMesh(const BezierSurface &bsurface, const std::vector<Texture>& textures) : m_textures(textures)
	 {
		m_vertices.reserve(16);
		for (const auto& row : bsurface)
			for (const auto& vertex : row)
				m_vertices.emplace_back(vertex);

		setupMesh();
	}


	std::vector<glm::vec3> m_vertices;
	std::vector<Texture> m_textures;

	void Draw(const Shader& shader) const;
private:
	unsigned int VAO, VBO;
	void setupMesh();


};