#pragma once
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniformMat4f(std::string name, glm::mat4 m);
	void SetUniform1f(std::string name, float f);
	void SetUniform4f(std::string name, float f0, float f1, float f2, float f3);
	unsigned int program;
private:
	int GetUniformLocation(std::string& name);
	unsigned int vertex;
	unsigned int fragment;
};