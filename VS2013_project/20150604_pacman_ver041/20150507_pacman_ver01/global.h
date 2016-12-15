#ifndef GLOBAL_H
#define GLOBAL_H

/* OpenGL 3rd-party libraries */
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <glm/glm.hpp>	/* OpenGL math library */
#include <glm/gtc/quaternion.hpp>
#include <SOIL.h>	/* Simple OpenGL Image Library */
#include "assimp/Importer.hpp"	/* Open asset import library */
#include "assimp/scene.h"
#include "assimp/postprocess.h"

/* std C++ libraries */
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>

/* global defines */
//#define M_PI       3.14159265358979323846
#define DEG_TO_RAD (3.14159265358979323846 / 180.0)	// constant: pi/180

enum CharactorStatus { MOVEING_FORWARD, TURNING_LEFT, TURNING_RIGHT, TURNING_BACK, IDLE };
enum Direction{ DOWN, LEFT, UP, RIGHT };

struct Location{
	int x;
	int y;
};

/* utility function prototypes (Definition in main.cpp) */
Location CreateLocation(int x, int y);
std::string Dir2String(Direction dir);

/* model class holds OpenGL rendering assests */
class Model{
private:
	GLuint shaderProgram_;
	GLuint vao_;
	std::vector<GLuint> vbo_;	// pointer to array, vbo maybe multiple
	GLuint ebo_;
	int indicesSize_;
	glm::vec3 color_;
	bool isUseTexture_;
	GLuint texture_;

public:
	Model(){ vbo_.resize(2); isUseTexture_ = false; }
	Model(const int nVbo){
		if (nVbo == 3){
			vbo_.resize(nVbo);
			isUseTexture_ = true;
		}
		else{
			vbo_.resize(2);
			isUseTexture_ = false;
		}
	}
	~Model(){}

	bool IsUseTexture(){ return isUseTexture_; }

	GLuint Get_shaderProgram(){ return shaderProgram_; }
	void Set_shaderProgram(GLuint shaderProgram){ shaderProgram_ = shaderProgram; }

	GLuint Get_vao(){ return vao_; }
	void Set_vao(GLuint vao){ vao_ = vao; }

	std::vector<GLuint> Get_vbo(){ return vbo_; }
	void Set_vbo(GLuint *vbo, int nVbo){ std::copy(vbo, vbo + nVbo, vbo_.begin()); }

	GLuint Get_ebo(){ return ebo_; }
	void Set_ebo(GLuint ebo){ ebo_ = ebo; }

	int Get_indicesSize(){ return indicesSize_; }
	void Set_indicesSize(int size){ indicesSize_ = size; }

	glm::vec3 Get_Color(){ return color_; }
	void Set_Color(glm::vec3 mColor){ color_ = mColor; }

	GLuint Get_texture(){ return texture_; }
	void Set_texture(GLuint texture){ texture_ = texture; }

	void CleanUp(){
		glDeleteVertexArrays(1, &vao_);
		const size_t nVbo_ = vbo_.size();
		glDeleteBuffers(nVbo_, vbo_.data());
		vao_ = 0;
		vbo_.clear();
	}
};

#endif
