#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;
	
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	Shader(const std::string &vertexPath, const std::string &tescPath, const std::string &tesePath, const std::string &fragmentPath);


	void use();
	
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int  value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4 &mat) const;
	//void compareMat4(const glm::mat4 &)

private:
	void checkCompileErrors(unsigned int shader, std::string type) const;
};


