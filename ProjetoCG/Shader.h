#pragma once
#include<string>
#include<glm.hpp>
#include<unordered_map>

class Shader
{
public:

	Shader();

	Shader(const std::string vertFilePath, const std::string fragFilePath);

	void CreateShaders();

	void Bind() const;
	void Unbind() const;

	void SendUniformData(const std::string& variableName, const glm::mat4& data);
	void SendUniformData(const std::string& variableName, const glm::vec4& data);

private:
	unsigned int _programId;
	unsigned int _vertexShaderId, _fragmentShaderId;
	std::string _vertFilePath;
	std::string _fragFilePath;
	std::unordered_map<std::string, int> _uniformLocation;

	void CompileShaders();
	void LinkProgram();

	int GetUniformLocation(const std::string& variableName);
	std::string ReadProgramSource(const std::string filePath);
};
