#include "Shader.h"
#include "glew.h"
#include "Log.h"

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
{
	program = glCreateProgram();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vertexSrc = vertexSource.c_str();
	const char* fragmentSrc = fragmentSource.c_str();
	glShaderSource(vertex, 1, &vertexSrc, nullptr);
	glShaderSource(fragment, 1, &fragmentSrc, nullptr);
	glCompileShader(vertex);
	glCompileShader(fragment);

	int compileStatus = 0;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) print("vertex shader compile error!");
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) print("fragment shader compile error!");

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glValidateProgram(program);
}
void Shader::SetUniform1f(std::string name, float f)
{
	glUniform1f(GetUniformLocation(name), f);
}
void Shader::SetUniformMat4f(std::string name, glm::mat4 m)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}
void Shader::SetUniform4f(std::string name, float f0, float f1, float f2, float f3)
{
	glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}
int Shader::GetUniformLocation(std::string& name)
{
	return glGetUniformLocation(program, name.c_str());
}
Shader::~Shader()
{
	glDeleteProgram(program);
}
void Shader::Bind() const
{
	glUseProgram(program);
}
void Shader::Unbind() const
{
	glUseProgram(0);
}