#include "gl_utils.h"


namespace gl_utils{
	void GetGlLog(GLuint parameter, GLenum flag){
		/* List of common pname for different return value
		GL_SHADER_TYPE, params returns GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
		GL_COMPILE_STATUS, params returns GL_TRUE or GL_FALSE
		GL_INFO_LOG_LENGTH, params returns log length, 0 if no log infor
		GL_SHADER_SOURCE_LENGTH, params returns  source length, 0 if code does not exist
		GL_COMPILE_STATUS, compile status */

		GLint params = GL_FALSE;	// false be defauld
		int max_length = 2048;	// shader log buffer size
		int actual_length = 0;	// ref actual buffer size
		char shader_log[2048];	// log data
		glGetShaderiv(parameter, GL_COMPILE_STATUS, &params);
		if (params == GL_FALSE)
		{
			glGetShaderInfoLog(parameter, max_length, &actual_length, shader_log);
			std::cerr << "ERROR: " << parameter << std::endl;
			std::cerr << shader_log << std::endl;
		}
	}

	void PrintGlInfro(){
		const GLubyte* renderer;
		const GLubyte* version;
		renderer = glGetString(GL_RENDERER);
		version = glGetString(GL_VERSION);
		std::cout << "Renderer: " << renderer << std::endl;
		std::cout << "OpenGL: " << version << std::endl;
	}

	std::string GetClientApiString(GLFWwindow *window){
		const int clientApi = glfwGetWindowAttrib(window, GLFW_CLIENT_API);
		const int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
		const int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
		std::string clientApiString;
		if (GLFW_OPENGL_API == clientApi) {
			clientApiString = "OpenGL";
		}
		else if (GLFW_OPENGL_ES_API == clientApi) {
			clientApiString = "OpenGL ES";
		}
		std::ostringstream sstream;
		sstream << clientApiString << " " << major << "." << minor;
		return sstream.str();
	}

	void GlErrorCheck(bool displayMessage){
		GLenum error = glGetError();	// clear error flag in eary code, add this part in eary of the code
		if (error != GL_NO_ERROR && displayMessage)
			std::cerr << "OpenGL Error " << error << std::endl;
	}

	void ConcatVectorGLfloat(std::vector<GLfloat> &base, std::vector<GLfloat> nextVector){
		base.reserve(base.size() + nextVector.size());
		base.insert(base.end(), nextVector.begin(), nextVector.end());
	}

	std::string ReadTextFile(const char *filePath){
		std::string str;
		std::fstream iFile(filePath, std::ios::in);
		if (!iFile.is_open()){
			std::cerr << "Error: Unable to open shader file" << std::endl;
			return NULL;
		}
		std::string line = "";
		while (!iFile.eof()){
			getline(iFile, line);
			str.append(line + "\n");
		}
		iFile.close();
		return str;
	}

	void CleanUp(GLFWwindow *window, Shader &shader){
		glfwDestroyWindow(window);
		shader.CleanUp();
		glfwTerminate();
	}

	void DepthTest(bool enable, double clearDepth, double near, double far){
		if (!enable) { return; }
		glEnable(GL_DEPTH_TEST);
		glClearDepth(clearDepth);
		glDepthFunc(GL_LESS);
		glDepthRange(near, far);	// Near, Far
	}

	void CullFace(bool enable, GLenum mode1, GLenum mode2){
		if (enable){
			glEnable(GL_CULL_FACE); // cull face
			glCullFace(mode1); // cull back face
			glFrontFace(mode2); // GL_CCW for counter clock-wise
		}
	}
}
