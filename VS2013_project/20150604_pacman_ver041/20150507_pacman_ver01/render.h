#ifndef RENDER_H
#define RENDER_H

#include "global.h"
#include "shader.h"

namespace render{
	bool InitializeWindowManager();
	GLFWwindow *CreateWindow(int width, int height, const char *title,
							GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL,
							int major = 4, int minor = 1);
	void LoadModelData(Model &model, std::vector<GLfloat> vertexs,
		std::vector<GLfloat> textureCoords, std::vector<GLuint> indices);
	void LoadModelData(Model &model, std::vector<GLfloat> vertexs,
		std::vector<GLfloat> normals, std::vector<GLuint> indices, std::vector<GLfloat> textureCoords);
	void RunRenderLoop(GLFWwindow *window, Shader &shader);
}
#endif
