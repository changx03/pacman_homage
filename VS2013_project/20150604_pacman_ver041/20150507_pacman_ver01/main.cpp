/*-------------------------------------------------
 *	Massey University, Albany, New Zealand
 *	Author Name: Xinglong Chang, ID: 06227663
 *	ate: 18-03-2015
 *	GLEW	version: 1.12.0
 *	GLFW	version: 3.1
 *	GLM		version: 0.9.6.3
 *	SOIL	version: July 7, 2008
 *	ASSIMP	version: 3.0 (win prebuild SDK, 07-2012)
 *
 *	Note: Compiled and tested on Visual Studio 2013
 *	Description: 
 *-------------------------------------------------*/

#include "global.h"
#include "render.h"
#include "gl_utils.h"
#include "shader.h"
#include "mesh_utils.h"
#include "world.h"
#include "imageLoader.h"

#define AI_PARAMTERS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs)

/* global models (Use as external in render.cpp) */
// map file for mini map 1
//World myPacmanWorld(CreateLocation(2, 3), "simpleMaze.txt");
// map file for larger map
World myPacmanWorld(CreateLocation(6, 7), "map3.txt");

std::vector<Model> groundModel;
std::vector<Model> skyboxModel;

/* function prototypes */
void GlobalModelCleanUp();
void PrintProgramControlIntro();

int main(){

	/* Initialize window */
	const glm::vec2 SCREEN_SIZE(800, 600);
	render::InitializeWindowManager();
	GLFWwindow *window = render::CreateWindow((int)SCREEN_SIZE.x,
		(int)SCREEN_SIZE.y, "OpenGL window");
	if (!window){
		std::cerr << "ERROR: GLFW3 Window initialization failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* print GL information */
	std::string clientApi = gl_utils::GetClientApiString(window);
	std::cout << "The client lib is: " << clientApi << std::endl;
	gl_utils::PrintGlInfro();

	/* Load shader */
	Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
	std::cout << shader.ToString() << std::endl;

	/* Initialize assimp */
	// test with blender monkey
	//const char *meshCharFileName = "monkey.obj";
	// my pacman model
	const char *meshCharFileName = "pacman_ver2_flipNormal.obj";
	
	const char *meshStarFileName = "star.obj";
	//const char *meshStarFileName = "monkey.obj";
	
	// use mini map
	//const char *meshFileName = "simpleMaze.obj";
	// use larger map
	const char *meshFileName = "maze3.obj";

	Assimp::Importer mImporterMaze;
	const aiScene *mScene = mImporterMaze.ReadFile(meshFileName, AI_PARAMTERS);
	Assimp::Importer mImporterCha;
	const aiScene *mChaScene = mImporterCha.ReadFile(meshCharFileName, AI_PARAMTERS);
	Assimp::Importer mImporterStar;
	const aiScene *mStarScene = mImporterStar.ReadFile(meshStarFileName, AI_PARAMTERS);

	const char *meshFile_ground = "ground.obj";
	Assimp::Importer mImporterGround;
	const aiScene *mGroundScene = mImporterGround.ReadFile(meshFile_ground, AI_PARAMTERS);

	const char *meshFile_skybox = "skybox01.obj";
	Assimp::Importer mImporterSkybox;
	const aiScene *mScene_skybox = mImporterSkybox.ReadFile(meshFile_skybox, AI_PARAMTERS);

	if (mChaScene == NULL || mScene == NULL || mStarScene == NULL || mGroundScene == NULL|| mScene_skybox == NULL){
		std::cerr << "Fail to import mesh file." << std::endl;
		return -1;
	}
	else
		std::cout << "Assimp is ready to go." << std::endl;

	/* print assimp import information*/
	mesh_utils::PrintSceneInfro(mScene);
	mesh_utils::PrintSceneInfro(mChaScene);
	mesh_utils::PrintSceneInfro(mStarScene);
	mesh_utils::PrintSceneInfro(mGroundScene);
	mesh_utils::PrintSceneInfro(mScene_skybox);

	/* Load meshes to model */
	std::vector<Model> mazeModel;
	std::vector<Model> characterModel;
	std::vector<Model> starModel;
	mazeModel = mesh_utils::LoadModels(shader, mScene);
	characterModel = mesh_utils::LoadModels(shader, mChaScene);
	starModel = mesh_utils::LoadModels(shader, mStarScene);

	/* Load models into myWorld*/
	// Note: use pass by ref to clear the obsolete vectors after set the model
	myPacmanWorld.SetMapModel(mazeModel);
	myPacmanWorld.SetMyCharacterModel(characterModel);
	myPacmanWorld.SetStarModel(starModel);

	/* add some star to our world */
	// stars for mini map
	//myPacmanWorld.AddStar(CreateLocation(1, 1), glm::vec3(-1.0f, 0.0f, -2.0f));
	//myPacmanWorld.AddStar(CreateLocation(3, 1), glm::vec3(1.0f, 0.0f, -2.0f));
	
	// stars for map2
	myPacmanWorld.AddStar(CreateLocation(5, 5), glm::vec3(-1.0f, 0.0f, -2.0f));
	myPacmanWorld.AddStar(CreateLocation(7, 5), glm::vec3(1.0f, 0.0f, -2.0f));
	myPacmanWorld.AddStar(CreateLocation(1, 1), glm::vec3(-5.0f, 0.0f, -6.0f));
	myPacmanWorld.AddStar(CreateLocation(11, 1), glm::vec3(5.0f, 0.0f, -6.0f));
	myPacmanWorld.AddStar(CreateLocation(1, 7), glm::vec3(-5.0f, 0.0f, 0.0f));
	myPacmanWorld.AddStar(CreateLocation(11, 7), glm::vec3(5.0f, 0.0f, 0.0f));
	myPacmanWorld.AddStar(CreateLocation(6, 1), glm::vec3(0.0f, 0.0f, -6.0f));

	groundModel = mesh_utils::LoadModels(shader, mGroundScene, true, true);
	skyboxModel = mesh_utils::LoadModels(shader, mScene_skybox, true, true);

	/* Load texture image */
	GLuint texture01;
	texture_utils::LoadTexture("ground.png", texture01);
	texture_utils::SetTextureFiltering(texture01, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture_utils::UpdateModelTexture(groundModel, texture01);
	GLuint texture02;
	texture_utils::LoadTexture("skybox01.png", texture02);
	texture_utils::SetTextureFiltering(texture02, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture_utils::UpdateModelTexture(skyboxModel, texture02);

	/* No need assimp later on */
	mImporterCha.FreeScene();
	mImporterMaze.FreeScene();
	mImporterStar.FreeScene();
	mImporterGround.FreeScene();
	mImporterSkybox.FreeScene();

	PrintProgramControlIntro();

	/* Start rendering */
	gl_utils::DepthTest(true, 1.0, 0.0, 1.0);
	gl_utils::CullFace(true, GL_BACK, GL_CCW);
	render::RunRenderLoop(window, shader);

	std::cout << "GLFW window closed. Cleaning up... (Press ENTER key to exit)" << std::endl;
	gl_utils::CleanUp(window, shader);
	GlobalModelCleanUp();

	return 0;
}

void GlobalModelCleanUp(){
	for (size_t i = 0; i < groundModel.size(); i++){
		groundModel[i].CleanUp();
	}
	for (size_t i = 0; i < skyboxModel.size(); i++){
		skyboxModel[i].CleanUp();
	}
}

void PrintProgramControlIntro(){
	printf("------------------------------------\n");
	printf("|          KEY CONTROL TABLE       |\n");
	printf("------------------------------------\n");
	printf("| %5s | %24s |\n", "KEY", "COMMAND");
	printf("------------------------------------\n");
	printf("| %5s | %24s |\n", "W", "Move forward");
	printf("| %5s | %24s |\n", "A", "Turn left");
	printf("| %5s | %24s |\n", "D", "Turn right");
	printf("| %5s | %24s |\n", "S", "Turn back");
	printf("| %5s | %24s |\n", "R", "Game reset");
	printf("| %5s | %24s |\n", "SPACE", "Game pause");
	printf("| %5s | %24s |\n", "[", "Camera  Y position ++");
	printf("| %5s | %24s |\n", "]", "Camera  Y position --");
	printf("| %5s | %24s |\n", "LEFT", "Camera  X position --");
	printf("| %5s | %24s |\n", "RIGHT", "Camera  X position ++");
	printf("| %5s | %24s |\n", "UP", "Camera  Z position ++");
	printf("| %5s | %24s |\n", "DOWN", "Camera  Z position ++");
	printf("------------------------------------\n");
}


// These methods are used globally
Location CreateLocation(int x, int y){
	Location locationBuffer;
	locationBuffer.x = x;
	locationBuffer.y = y;
	return locationBuffer;
}

std::string Dir2String(Direction dir){
	std::string dirTempStr;
	switch (dir){
	case DOWN:
		dirTempStr = "DOWN";
		break;
	case LEFT:
		dirTempStr = "LEFT";
		break;
	case UP:
		dirTempStr = "UP";
		break;
	case RIGHT:
		dirTempStr = "RIGHT";
		break;
	default:
		dirTempStr = "NULL";
		break;
	}
	return dirTempStr;
}