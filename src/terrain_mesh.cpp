#include <terrain_mesh.h>
#include <shader.h>
#include <glad/glad.h> 

void TerrainMesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
}

void TerrainMesh::Draw(const Shader& shader) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int displacementNr = 1;
    unsigned int blendNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); 
        else if (name == "texture_height")
            number = std::to_string(heightNr++); 
        else if (name == "texture_displacement")
            number = std::to_string(displacementNr++); 
        else if (name == "texture_blend")
            number = std::to_string(blendNr++); 

        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_PATCHES, 0, 16);
    glBindVertexArray(0);

}