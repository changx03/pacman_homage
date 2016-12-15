#include "render.h"
#include "gl_utils.h"
#include "math_matrix.h"
#include "world.h"

extern World myPacmanWorld;
extern std::vector<Model> groundModel;
extern std::vector<Model> skyboxModel;

namespace render{
	namespace{
		/* local global variables for rendering */
		glm::mat4 chaTranslation;
		glm::mat4 chaRotation;
		float moveAccumulator = 0.0f;
		float angleAccumulator = 0.0f;

		glm::vec3 camPos;
		// Initialize camera position
		// for mini map
		//const glm::vec3 initialCam(0.0f, 3.5f, 0.5f);
		// for map2
		const glm::vec3 initialCam(0.0f, 3.8f, 0.3f);
		const glm::vec3 initialCam0(0.0f, 15.6f, 0.3f);

		static bool isPause;

		void errorCallback(int error, const char *description){
			std::cerr << "ERROR " << error << ": " << description << std::endl;
		}

		void FramebufferSizeCallback(GLFWwindow *window, int width, int height){
			glfwMakeContextCurrent(window);
			glViewport(0, 0, width, height);
			glfwMakeContextCurrent(NULL);
		}

		void WindowCloseCallback(GLFWwindow *window){
			// Nothing to do right now
		}

		void UpdatingCharactor(){
			float angle = 0.0f;
			float move = 0.0f;
			glm::mat4 rotation;
			glm::mat4 nextMoveMat;
			switch (myPacmanWorld.GetCurrentStatus()) {
			case TURNING_LEFT:
				angle = 7.5f;
				angleAccumulator += angle;
				rotation = math_matrix::Rotate(0.0f, angle, 0.0f);
				chaRotation = chaRotation * rotation;
				if (std::round(angleAccumulator) == 90.0f){	// turning completed
					myPacmanWorld.SetCurrentStatus(IDLE);
					angleAccumulator = 0.0f;
				}
				break;
			case TURNING_RIGHT:
				angle = -7.5f;
				angleAccumulator += angle;
				rotation = math_matrix::Rotate(0.0f, angle, 0.0f);
				chaRotation = chaRotation * rotation;
				if (std::round(angleAccumulator) == -90.0f){	// turning completed
					myPacmanWorld.SetCurrentStatus(IDLE);
					angleAccumulator = 0.0f;
				}
				break;
			case TURNING_BACK:
				angle = 15.0f;
				angleAccumulator += angle;
				rotation = math_matrix::Rotate(0.0f, angle, 0.0f);
				chaRotation = chaRotation * rotation;
				if (std::round(angleAccumulator) == 180.0f){	// turning completed
					myPacmanWorld.SetCurrentStatus(IDLE);
					angleAccumulator = 0.0f;
				}
				break;
			case MOVEING_FORWARD:
				switch (myPacmanWorld.GetCurrentDirection()){
				case DOWN:
					move = 0.2f;
					nextMoveMat = math_matrix::GetModelTranslation(glm::vec3(0.0f, 0.0f, move));
					break;
				case LEFT:
					move = 0.2f;
					nextMoveMat = math_matrix::GetModelTranslation(glm::vec3(move, 0.0f, 0.0f));
					break;
				case UP:
					move = -0.2f;
					nextMoveMat = math_matrix::GetModelTranslation(glm::vec3(0.0f, 0.0f, move));
					break;
				case RIGHT:
					move = -0.2f;
					nextMoveMat = math_matrix::GetModelTranslation(glm::vec3(move, 0.0f, 0.0f));
					break;
				default:
					move = 1.0f;
					break;
				}

				chaTranslation = nextMoveMat * chaTranslation;
				moveAccumulator = moveAccumulator + fabs(move);
				if (moveAccumulator >= 1.0f){
					myPacmanWorld.SetCurrentStatus(IDLE);
					moveAccumulator = 0.0f;
				}

				break;
			default:	// case IDLE
				angle = 0.0f;
				move = 0.0f;
				angleAccumulator = 0.0f;
				moveAccumulator = 0.0f;
				myPacmanWorld.SetCurrentStatus(IDLE);
				break;
			}
		}
		
		void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
				Location myLocation = myPacmanWorld.GetMyLocation();
				std::cout << "(" << myLocation.x << ", " << myLocation.y << ")" << std::endl;
				switch (key) {
				case GLFW_KEY_ESCAPE:	// exit
						glfwSetWindowShouldClose(window, GL_TRUE);
						WindowCloseCallback(window);
					break;
				case GLFW_KEY_R:	// reset
					if (action == GLFW_PRESS) {
						std::cout << "Game has been reset." << std::endl;
						myPacmanWorld.Reset();
						chaTranslation = glm::mat4(1.0f);
						chaRotation = glm::mat4(1.0f);
						camPos = initialCam;
					}
					break;
				case GLFW_KEY_W:
					//if (action == GLFW_PRESS) {
						if (myPacmanWorld.GetCurrentStatus() == IDLE && myPacmanWorld.IsNextPositionAccessible()){
							std::cout << "Let's move! :D" << std::endl;
							myPacmanWorld.SetCurrentStatus(MOVEING_FORWARD);
							myPacmanWorld.UpdateMyLocation();
							for (size_t i = 0; i < myPacmanWorld.GetStars().size(); i++){
								if (myPacmanWorld.GetMyLocation().x == myPacmanWorld.GetStars()[i].mLocation.x &&
									myPacmanWorld.GetMyLocation().y == myPacmanWorld.GetStars()[i].mLocation.y){
									myPacmanWorld.UpdateStar(i);
								}
							}
						}
						else{
							std::cout << "I can't move right now!" << std::endl;
						}
					//}
					break;
				case GLFW_KEY_A:
					if (action == GLFW_PRESS) {
						if (myPacmanWorld.GetCurrentStatus() == IDLE){
							std::cout << "Let's move left! :D" << std::endl;
							myPacmanWorld.SetCurrentStatus(TURNING_LEFT);
							myPacmanWorld.UpdateCurrentDirection(LEFT);
						}
						else{
							std::cout << "Sorry. I'm busy... :(" << std::endl;
						}
					}
					break;
				case GLFW_KEY_D:
					if (action == GLFW_PRESS) {
						if (myPacmanWorld.GetCurrentStatus() == IDLE){
							std::cout << "Let's move right! :D" << std::endl;
							myPacmanWorld.SetCurrentStatus(TURNING_RIGHT);
							myPacmanWorld.UpdateCurrentDirection(RIGHT);
						}
						else{
							std::cout << "Sorry. I'm busy... :(" << std::endl;
						}
					}
					break;
				case GLFW_KEY_S:
					if (action == GLFW_PRESS) {
						if (myPacmanWorld.GetCurrentStatus() == IDLE){
							std::cout << "Let's turn 180 degree! :D" << std::endl;
							myPacmanWorld.SetCurrentStatus(TURNING_BACK);
							myPacmanWorld.UpdateCurrentDirection(DOWN);
						}
						else{
							std::cout << "Sorry. I'm busy... :(" << std::endl;
						}
					}
					break;
				case GLFW_KEY_SPACE:
					if (action == GLFW_PRESS) {
						std::cout << "isPause " << isPause << std::endl;
						isPause = !isPause;
					}
					break;
				case GLFW_KEY_LEFT:
					camPos.x = camPos.x - 0.1f;
					std::cout << "CamPosX " << camPos.x << std::endl;
					break;
				case GLFW_KEY_RIGHT:
					camPos.x = camPos.x + 0.1f;
					std::cout << "CamPosX " << camPos.x << std::endl;
					break;
				case GLFW_KEY_UP:
					camPos.z = camPos.z + 0.1f;
					std::cout << "CamPosZ " << camPos.z << std::endl;
					break;
				case GLFW_KEY_DOWN:
					camPos.z = camPos.z - 0.1f;
					std::cout << "CamPosZ " << camPos.z << std::endl;
					break;
				case GLFW_KEY_LEFT_BRACKET:
					camPos.y = camPos.y - 0.1f;
					std::cout << "CamPosY " << camPos.y << std::endl;
					break;
				case GLFW_KEY_RIGHT_BRACKET:
					camPos.y = camPos.y + 0.1f;
					std::cout << "CamPosY " << camPos.y << std::endl;
					break;
				}
		}

		void Window_size_callbak(GLFWwindow *window, int width, int height){
			std::cout << "New window size: w = " << width << ", h = " << height << std::endl;
		}

		void DrawModels(std::vector<Model> models, Shader shader){
			size_t numModels = models.size();
			for (size_t i = 0; i < numModels; i++){
				//glUseProgram(shader.GetProgram());

				// update model color
				glm::vec3 mColor = models[i].Get_Color();
				const GLint modelColorLoc = glGetUniformLocation(shader.GetProgram(), "modelColor");
				glUniform3f(modelColorLoc, mColor.r, mColor.g, mColor.b);

				glBindVertexArray(models[i].Get_vao());
				/* Position attribute */
				GLint attribVertex = 0;
				glEnableVertexAttribArray(attribVertex);
				glBindBuffer(GL_ARRAY_BUFFER, (models[i].Get_vbo())[0]);
				glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

				/* Normal attribute */
				GLint attribNormal = 1;
				glEnableVertexAttribArray(attribNormal);
				glBindBuffer(GL_ARRAY_BUFFER, (models[i].Get_vbo())[1]);
				glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

				GLuint colorIdx;
				if (models[i].IsUseTexture()){
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, models[i].Get_texture());
					const GLint texUnitLocation = glGetUniformLocation(shader.GetProgram(), "mTexture");
					glUniform1i(texUnitLocation, 0);	// using GL_TEXTURE0

					GLint attribTexCoord = 2;
					glEnableVertexAttribArray(attribTexCoord);
					glBindBuffer(GL_ARRAY_BUFFER, (models[i].Get_vbo())[2]);
					glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

					colorIdx = glGetSubroutineIndex(shader.GetProgram(), GL_FRAGMENT_SHADER, "UseTexture");
					glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorIdx);
				}
				else{
					colorIdx = glGetSubroutineIndex(shader.GetProgram(), GL_FRAGMENT_SHADER, "UseModelColor");
					glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorIdx);
				}

				/* Draw */
				glDrawElements(GL_TRIANGLES, models[i].Get_indicesSize(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

			}
		}

		void UpdateModelMat(glm::mat4 *modelMat, Shader shader){
			glm::mat4 model = *modelMat;
			const GLint modelMatrixLocation = glGetUniformLocation(shader.GetProgram(), "modelMat");
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &(model[0].x));
		}
	}

	bool InitializeWindowManager(){
		glfwSetErrorCallback(errorCallback);
		if (!glfwInit()){ return false; }
		return true;
	}

	GLFWwindow *CreateWindow(int width, int height, const char *title,
		GLFWmonitor *monitor, GLFWwindow *share, int major, int minor){
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// create the window
		GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);
		if (!window) { return NULL; }

		// initialise GLEW
		glfwMakeContextCurrent(window);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) { return NULL; }

		// set window callback functions
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetWindowSizeCallback(window, Window_size_callbak);

		gl_utils::GlErrorCheck(false);	// clear error flag
		return window;
	}

	void LoadModelData(Model &model, std::vector<GLfloat> vertexs,
		std::vector<GLfloat> normals, std::vector<GLuint> indices){
		GLuint vao, ebo;
		GLuint vbo[2];

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		model.Set_vao(vao);

		glGenBuffers(2, vbo);	// 2 vbo: vertex and normal
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, (vertexs.size() * sizeof(GLfloat)), vertexs.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, (normals.size() * sizeof(GLfloat)), normals.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

		model.Set_vbo(vbo, 2);
		model.Set_ebo(ebo);
		/* unbind vao */
		glBindVertexArray(0);
		//gl_utils::GlErrorCheck(true);
	}

	void LoadModelData(Model &model, std::vector<GLfloat> vertexs,
		std::vector<GLfloat> normals, std::vector<GLuint> indices, std::vector<GLfloat> textureCoords){
		GLuint vao, ebo;
		GLuint vbo[3];

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		model.Set_vao(vao);

		glGenBuffers(3, vbo);
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, (vertexs.size() * sizeof(GLfloat)), vertexs.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, (normals.size() * sizeof(GLfloat)), normals.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, (textureCoords.size() * sizeof(GLfloat)), textureCoords.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

		model.Set_vbo(vbo, 3);
		model.Set_ebo(ebo);

		glBindVertexArray(0);
	}

	void RunRenderLoop(GLFWwindow *window, Shader &shader){
		// Load program before update matrix
		//std::cout << shader.GetProgram() << std::endl;
		glUseProgram(shader.GetProgram());

		// Initialize light position
		glm::vec3 lightPos_eye(0.0f, 5.0f, -2.0f);
		const GLint lightPosLoc = glGetUniformLocation(shader.GetProgram(), "lightPos_eye");
		glUniform3f(lightPosLoc, lightPos_eye.x, lightPos_eye.y, lightPos_eye.z);

		// Initialize projection matrix
		glm::mat4 projection;		
		// Initialize camera position
		camPos = initialCam0;
		chaTranslation = math_matrix::GetModelTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 modelMat, rotation, scale;
		scale = math_matrix::GetScaleTransform(0.5f);
		glm::mat4 chaMat = chaRotation * chaTranslation * scale;

		std::cout << "initial dir: " << Dir2String(myPacmanWorld.GetCurrentDirection()) << std::endl;

		// set initial location
		Location myLocation = myPacmanWorld.GetMyLocation();
		std::cout << "(" << myLocation.x << ", " << myLocation.y << ")" << std::endl;
		
		isPause = false;
		bool isStart = true;
		double time1 = glfwGetTime();
		while (!glfwWindowShouldClose(window)) {	// the main render loop
			glfwPollEvents();	// poll window events
			glfwMakeContextCurrent(window);	// make the context of the given window current on the calling thread
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);	// Set back ground color to grey
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			double time = glfwGetTime();

			// enter sence animation
			if (camPos.y > initialCam.y && isStart)
				camPos.y = camPos.y - 0.1f;
			else
				isStart = false;

			// update view matrix
			glm::mat4 view = math_matrix::GetViewTransform(camPos,
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));

			// update projection matrix
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			GLfloat windowRatio = width / (GLfloat)height;
			projection = math_matrix::GetPerspectiveTransform(65.0f, windowRatio, 0.1f, 100.f);
			glm::mat4 camara = projection * view;
			const GLint camaraMatLocation = glGetUniformLocation(shader.GetProgram(), "camera");
			glUniformMatrix4fv(camaraMatLocation, 1, GL_FALSE, &(camara[0].x));

			// start rendering
			// maze
			UpdateModelMat(&scale, shader);
			DrawModels(myPacmanWorld.GetMapModel(), shader);
			DrawModels(groundModel, shader);
			DrawModels(skyboxModel, shader);

			// pacman
			UpdatingCharactor();
			chaMat = chaTranslation * chaRotation * scale;
			UpdateModelMat(&chaMat, shader);
			DrawModels(myPacmanWorld.GetMyCharacterModel(), shader);

			// star
			rotation = math_matrix::GetRotateYAxis(time);
			for (size_t i = 0; i < myPacmanWorld.GetStars().size(); i++){
				if (myPacmanWorld.GetStars()[i].isShow){
					glm::mat4 translation = myPacmanWorld.GetStars()[i].mTranslationMat;
					modelMat = translation * rotation * scale;
					UpdateModelMat(&modelMat, shader);
					DrawModels(myPacmanWorld.GetStarModel(), shader);
				}
			}

			glfwSwapBuffers(window);	// swap the back and front buffers
			
			if (isPause){	// pause event
				glfwWaitEvents();
				glfwSetTime(time1);
			}
			else
				time1 = glfwGetTime();

			gl_utils::GlErrorCheck(true);
		}
		glfwMakeContextCurrent(NULL); // detach the context from the current thread
	}
}
