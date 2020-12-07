#pragma once
#include <string>
#include <vector>

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

unsigned int TextureFromFile(const std::string &path, bool gamma);
unsigned int loadCubemapTexture(std::vector<std::string> faces);
Texture loadMatTexture(const std::string &path, std::string typeName, bool gamma = false);