
#include <shader.h>


#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>


#include "helpers/RootDir.h"
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{

		vShaderFile.open(ROOT_DIR + vertexPath);
		fShaderFile.open(ROOT_DIR + fragmentPath);

		
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

Shader::Shader(const std::string& vertexPath, const std::string& tescPath, const std::string& tesePath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string tescCode;
	std::string teseCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream tescShaderFile;
	std::ifstream teseShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tescShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	teseShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(ROOT_DIR + vertexPath);
		tescShaderFile.open(ROOT_DIR + tescPath);
		teseShaderFile.open(ROOT_DIR + tesePath);
		fShaderFile.open(ROOT_DIR + fragmentPath);

		std::stringstream vShaderStream, tescShaderStream, teseShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		tescShaderStream << tescShaderFile.rdbuf();
		teseShaderStream << teseShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		tescShaderFile.close();
		teseShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		tescCode = tescShaderStream.str();
		teseCode = teseShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* tescShaderCode = tescCode.c_str();
	const char* teseShaderCode = teseCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	unsigned int tescShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tescShader, 1, &tescShaderCode, NULL);
	glCompileShader(tescShader);
	checkCompileErrors(tescShader, "TESSELATION CONTROL");

	unsigned int teseShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(teseShader, 1, &teseShaderCode, NULL);
	glCompileShader(teseShader);
	checkCompileErrors(teseShader, "TESSELATION EVALUATION");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");



	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, tescShader);
	glAttachShader(ID, teseShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(tescShader);
	glDeleteShader(teseShader);
	glDeleteShader(fragmentShader);


}

void Shader::use()
{
	glUseProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()) , value.x, value.y, value.z, value.w);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::checkCompileErrors(unsigned int shader, std::string type) const
{
	int success;
	char infoLog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}