#include <texture.h>
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "helpers/RootDir.h"


unsigned int TextureFromFile(const std::string &path, bool gamma)
{

	const auto directory = ROOT_DIR + path;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(directory.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum format;
		if (nrComponents == 1)
		{
			internalFormat = format = GL_RED;
		}

		else if (nrComponents == 3 || nrComponents == 2)
		{
			internalFormat = gamma ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << directory << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemapTexture(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		auto path = ROOT_DIR + faces[i];
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}



Texture loadMatTexture(const std::string &path, std::string typeName, bool gamma)
{
	static std::unordered_map<std::string, Texture> texture_index;

	auto file_name = path.substr(path.find_last_of('/') + 1);
	// return texture if previously loaded
	auto iter = texture_index.find(file_name);
	if (iter != texture_index.end())
		return iter->second;

	Texture texture;
	texture.id = TextureFromFile(path, gamma);
	texture.type = typeName;
	texture.path = path;
	texture_index[file_name] = texture;
	return texture;


}
