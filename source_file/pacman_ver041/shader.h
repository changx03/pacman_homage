#ifndef SHADER_H
#define SHADER_H

#include "global.h"

class Shader{
private:
	GLuint vs_;
	GLuint fs_;
	GLuint program_;
	std::string vertexStr_;
	std::string fragmentStr_;
public:
	Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
	GLuint GetProgram(){ return program_; }
	std::string ToString();
	void CleanUp();
};
#endif
