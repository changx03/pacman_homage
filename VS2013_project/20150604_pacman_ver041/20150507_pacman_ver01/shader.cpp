#include "shader.h"
#include "gl_utils.h"	// method ReadTxtFile

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath){
	vertexStr_ = gl_utils::ReadTextFile(vertexShaderPath);
	fragmentStr_ = gl_utils::ReadTextFile(fragmentShaderPath);
	
	const GLchar *vertex_shader = vertexStr_.c_str();
	const GLchar *fragment_shader = fragmentStr_.c_str();

	vs_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_, 1, &vertex_shader, NULL);
	glCompileShader(vs_);
	gl_utils::GetGlLog(vs_, GL_COMPILE_STATUS);

	fs_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_, 1, &fragment_shader, NULL);
	glCompileShader(fs_);
	gl_utils::GetGlLog(fs_, GL_COMPILE_STATUS);

	program_ = glCreateProgram();
	glAttachShader(program_, fs_);
	glAttachShader(program_, vs_);
	glLinkProgram(program_);
}

std::string Shader::ToString(){
	std::string temp_;
	temp_ = "ShaderProgram = " + std::to_string(program_) +
		", vs = " + std::to_string(vs_) + ", fs = " + std::to_string(fs_) + "\n" +
		"------------------VERTEX SHADER------------------\n" + 
		vertexStr_ + 
		"\n----------------FRAGMENT SHADER------------------\n" + 
		fragmentStr_ + "\n";
	return temp_;
}

void Shader::CleanUp(){
	glDeleteShader(vs_);
	glDeleteShader(fs_);
	glDeleteProgram(program_);
	vs_ = fs_ = program_ = 0;
}
