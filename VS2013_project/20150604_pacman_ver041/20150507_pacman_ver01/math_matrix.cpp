#include "math_matrix.h"

namespace math_matrix{
	glm::mat4 GetModelTranslation(glm::vec3 xyz){
		glm::mat4 translation(1.0f);
		translation[3][0] = xyz[0];
		translation[3][1] = xyz[1];
		translation[3][2] = xyz[2];
		return translation;
		;
	}

	glm::mat4 GetRotateYAxis(double speed){
		glm::mat4 rotate(1.0f);
		const GLfloat theta = (GLfloat)speed * 50.0f * (GLfloat)DEG_TO_RAD;
		GLfloat cosTheta = cos(theta);
		GLfloat sinTheta = sin(theta);
		rotate[0][0] = cosTheta;
		rotate[2][0] = sinTheta;
		rotate[0][2] = -sinTheta;
		rotate[2][2] = cosTheta;
		return rotate;
	}

	glm::mat4 GetScaleTransform(GLfloat x, GLfloat y, GLfloat z){
		glm::mat4 scale(1.0f);
		scale[0][0] = x;
		scale[1][1] = y;
		scale[2][2] = z;
		return scale;
	}

	glm::mat4 GetScaleTransform(GLfloat rate){
		glm::mat4 scale(1.0f);
		scale[0][0] = scale[1][1] = scale[2][2] = rate;
		return scale;
	}

	glm::mat4 GetViewTransform(const glm::vec3 camPos, const glm::vec3 lookAt, const glm::vec3 camUp0){
		glm::mat4 view;
		const glm::vec3 camOffset = lookAt - camPos;
		const glm::vec3 camForward = camOffset / glm::length(camOffset);
		const glm::vec3 camRight = glm::cross(camForward, camUp0);
		const glm::vec3 camUp = glm::cross(camRight, camForward);
		const glm::mat4 viewTranslation(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-camPos.x, -camPos.y, -camPos.z, 1);
		const glm::mat4 viewRotation(
			camRight.x, camUp.x, -camForward.x, 0.0f,
			camRight.y, camUp.y, -camForward.y, 0.0f,
			camRight.z, camUp.z, -camForward.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		view = viewRotation * viewTranslation;
		return view;
	}

	glm::mat4 GetPerspectiveTransform(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar){
		const GLfloat val_f = (GLfloat)(1.0f / (tan((fovy * DEG_TO_RAD) / 2.0f)));
		glm::mat4 perspective(0.0f);
		perspective[0][0] = val_f / aspect;
		perspective[1][1] = val_f;
		perspective[2][2] = (zFar + zNear) / (zNear - zFar);
		perspective[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
		perspective[2][3] = -1.0f;
		return perspective;
	}

	glm::mat4 Rotate(GLfloat x_deg, GLfloat y_deg, GLfloat z_deg){
		glm::vec3 angle_xyz(x_deg * DEG_TO_RAD, y_deg * DEG_TO_RAD, z_deg * DEG_TO_RAD);
		//glm::quat mQuaternion = glm::angleAxis((GLfloat)M_PI, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat mQuaternion(angle_xyz);
		mQuaternion = glm::normalize(mQuaternion);
		glm::mat4 rotation = glm::mat4_cast(mQuaternion);
		return rotation;
	}
}
