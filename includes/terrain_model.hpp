#pragma once
#include <terrain_mesh.h>
#include <shader.h>
#include <glm/gtc/matrix_transform.hpp>

class TerrainModel
{
public:
	TerrainModel(std::vector<TerrainMesh>&& meshes, bool gamma = false) : m_meshes(std::move(meshes)), m_worldPos(0.0), m_worldScale(1.0) {};
	
	glm::vec3 m_worldPos;
	glm::vec3 m_worldScale;

	void setPos(const glm::vec3& pos) {m_worldPos = pos; };
	void setScale(const glm::vec3& scale) { m_worldScale = scale; };
	void updatePos(const glm::vec3& pos) { m_worldPos += pos; };

	void Draw(const Shader& shader) const
	{
		constexpr glm::mat4 modelMat = glm::mat4(1.0f);
		auto worldMat = glm::translate(modelMat, m_worldPos);
		worldMat = glm::scale(worldMat, m_worldScale);

		shader.setMat4("model", worldMat);
		for (const auto& Mesh : m_meshes)
			Mesh.Draw(shader);
	}

private:
	std::vector<TerrainMesh> m_meshes;
};