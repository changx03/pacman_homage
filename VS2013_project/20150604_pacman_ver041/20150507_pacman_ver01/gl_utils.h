#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "global.h"
#include "shader.h"

namespace gl_utils{
	std::string GetClientApiString(GLFWwindow *window);
	void CheckShaderError(GLuint vs, GLuint fs);
	void GlErrorCheck(bool displayMessage);
	void GetGlLog(GLuint parameter, GLenum flag);
	void PrintGlInfro();
	void ConcatVectorGLfloat(std::vector<GLfloat> &base, std::vector<GLfloat> nextVector);
	std::string ReadTextFile(const char *filePath);
	void CleanUp(GLFWwindow *window, Shader &shader);
	void DepthTest(bool enable, double clearDepth, double near, double far);
	void CullFace(bool enable, GLenum mode1, GLenum mode2);
}
#endif
