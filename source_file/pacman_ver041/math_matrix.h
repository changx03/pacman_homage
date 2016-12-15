#ifndef MATH_MATRIX
#define MATH_MATRIX

#include "global.h"

namespace math_matrix{
	glm::mat4 GetModelTranslation(glm::vec3 xyz);
	glm::mat4 GetViewTransform(const glm::vec3 camPos, const glm::vec3 lookAt, const glm::vec3 camUp0);
	glm::mat4 GetPerspectiveTransform(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar);
	glm::mat4 GetRotateYAxis(double speed);
	glm::mat4 GetScaleTransform(GLfloat x, GLfloat y, GLfloat z);
	glm::mat4 GetScaleTransform(GLfloat rate);
	glm::mat4 Rotate(GLfloat x_deg, GLfloat y_deg, GLfloat z_deg);
}
#endif
