#include "mesh_utils.h"
#include "render.h"

namespace mesh_utils{
	void PrintSceneInfro(const aiScene *mScene){
		std::cout << "-------------PRINT SCENE INFORMATION-------------\n";

		std::cout << "lights = " << mScene->mNumLights << std::endl;
		std::cout << "material = " << mScene->mNumMaterials << std::endl;
		std::cout << "texture = " << mScene->mNumTextures << std::endl;
		std::cout << "meshes = " << mScene->mNumMeshes << std::endl;

		/* print vertices */
		for (size_t i = 0; i < mScene->mNumMeshes; i++){
			size_t vertexBufferLength = mScene->mMeshes[i]->mNumVertices * 3;
			std::cout << "vertices[" << i << "] = " << vertexBufferLength << std::endl;
		}

		/* print materials */
		// 1st material always has gray color. No use!
		for (size_t i = 0; i < mScene->mNumMaterials; i++){
			aiString mName;
			aiColor4D mColor(0.0f, 0.0f, 0.0f, 0.0f);
			aiGetMaterialColor(mScene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &mColor);
			std::cout << "Index[" << i << "] R = " << mColor.r << " G = " << mColor.g << " B = " << mColor.b << std::endl;
		}

		std::cout << "-------------------------------------------------\n";
	}

	void LoadMeshVertices(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &vertices){
		// incorrect mesh index number or fail to load scene
		if (meshIdx >= scene->mNumMeshes){
			std::cerr << "Error: Incorrect mesh index or invalid scene pointer. Exit" << std::endl;
			return;
		}

		// reserve memory
		vertices.clear();
		const size_t vertexBufferLength = scene->mMeshes[meshIdx]->mNumVertices;
		vertices.reserve(vertexBufferLength * 3);

		// load data
		for (size_t i = 0; i < vertexBufferLength; i++){
			const aiVector3D &tempVertex = scene->mMeshes[meshIdx]->mVertices[i];
			GLfloat x = tempVertex.x;
			GLfloat y = tempVertex.y;
			GLfloat z = tempVertex.z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	void LoadMeshTextureCoord(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &textureCoords){
		if (meshIdx >= scene->mNumMeshes){
			std::cerr << "Error: Incorrect mesh index or invalid scene pointer. Exit" << std::endl;
			return;
		}
		if (scene->mMeshes[meshIdx]->HasTextureCoords(0) == false){
			std::cerr << "Error: The mesh[" << meshIdx << "] has no texture coords. Exit" << std::endl;
			return;
		}
		// reserve memory
		textureCoords.clear();
		const size_t length = scene->mMeshes[meshIdx]->mNumVertices;
		textureCoords.reserve(length * 2);	// 
		for (size_t i = 0; i < length; i++){
			const aiVector3D &tempTextureCoord = scene->mMeshes[meshIdx]->mTextureCoords[0][i];
			//std::cout << tempTextureCoord.x << std::endl;
			//std::cout << tempTextureCoord.y << std::endl;
			//std::cout << tempTextureCoord.z << std::endl;	// constant '0'?
			
			GLfloat u = tempTextureCoord.x;
			GLfloat v = tempTextureCoord.y;
			textureCoords.push_back(u);
			textureCoords.push_back(v);

			//getchar();
		}
	}

	// Note: material index = mesh index - 1. materials[0].color always equals to 0.6 
	void LoadMaterialColor(const aiScene *scene, size_t meshIdx, glm::vec3 &color){
		// incorrect material index number or fail to load scene
		if (meshIdx >= scene->mNumMeshes){
			std::cerr << "Error: Incorrect material index or invalid scene pointer. Exit" << std::endl;
			exit(0);
		}

		size_t matIdx = scene->mMeshes[meshIdx]->mMaterialIndex;
		if (matIdx == 0){	// if material is empty, this color will return 0.6 (gray)
			std::cerr << "Error: the input mesh has no associated material. Return BLACK color." << std::endl;
			color = glm::vec3(0.6f, 0.6f, 0.6f);
			return;
		}

		aiColor4D mColor(0.0f, 0.0f, 0.0f, 0.0f);
		aiGetMaterialColor(scene->mMaterials[matIdx], AI_MATKEY_COLOR_DIFFUSE, &mColor);
		color = glm::vec3(mColor.r, mColor.g, mColor.b);
	}

	void LoadMeshNormals(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &normals){
		// incorrect mesh index number or fail to load scene
		if (meshIdx >= scene->mNumMeshes || scene->mMeshes[meshIdx]->mNormals == NULL){
			std::cerr << "Error: Unable to load normals. Exit" << std::endl;
			return;
		}

		// reserve memory
		normals.clear();
		const size_t normalBufferLength = scene->mMeshes[meshIdx]->mNumVertices;
		normals.reserve(normalBufferLength * 3);

		// load data
		for (size_t i = 0; i < normalBufferLength; i++){
			aiVector3D tempNormal = scene->mMeshes[meshIdx]->mNormals[i];
			normals.push_back(tempNormal.x);
			normals.push_back(tempNormal.y);
			normals.push_back(tempNormal.z);
		}
	}

	void LoadMeshIdx(const aiScene *scene, size_t meshIdx, std::vector<GLuint> &indices){
		// check mesh
		if (meshIdx >= scene->mNumMeshes){
			std::cerr << "Error: Incorrect material index or invalid scene pointer. Exit" << std::endl;
			return;
		}
		// check faces
		if (scene->mMeshes[meshIdx]->mFaces == NULL){
			std::cerr << "Erroc: Cannot load faces in mesh " << meshIdx << ". Exit." << std::endl;
			return;
		}

		// clean previous data
		indices.clear();

		// load indices
		for (size_t i = 0; i < scene->mMeshes[meshIdx]->mNumFaces; i++){
			const aiFace &face = scene->mMeshes[meshIdx]->mFaces[i];
			indices.insert(indices.end(), &face.mIndices[0], &face.mIndices[face.mNumIndices]);
		}
	}

	std::vector<Model> LoadModels(Shader shader, const aiScene *scene, bool isColor, bool isTexture){
		std::vector<Model> modelsOut;
		// initialize buffer
		const size_t numMesh = scene->mNumMeshes;
		std::vector<GLfloat> verticesBuffer;
		std::vector<GLfloat> normalsBuffer;
		std::vector<GLfloat> textureBuffer;	// no use in pacman 
		std::vector<GLuint> indicesBuffer;
		glm::vec3 colorBuffer;

		for (size_t idx = 0; idx < numMesh; idx++){
			LoadMeshVertices(scene, idx, verticesBuffer);
			LoadMeshNormals(scene, idx, normalsBuffer);
			LoadMeshIdx(scene, idx, indicesBuffer);
			if (isColor)
				LoadMaterialColor(scene, idx, colorBuffer);
			else
				colorBuffer = glm::vec3(0.6f, 0.6f, 0.6f);	// no color, set color value to gray	

			Model modelBuffer;
			if (isTexture == false){
				render::LoadModelData(modelBuffer, verticesBuffer, normalsBuffer, indicesBuffer);
			}
			else{
				modelBuffer = Model(3);
				LoadMeshTextureCoord(scene, idx, textureBuffer);
				render::LoadModelData(modelBuffer, verticesBuffer, normalsBuffer, indicesBuffer, textureBuffer);
			}

			modelBuffer.Set_shaderProgram(shader.GetProgram());
			modelBuffer.Set_Color(colorBuffer);
			modelBuffer.Set_indicesSize(indicesBuffer.size());

			verticesBuffer.clear();
			normalsBuffer.clear();
			textureBuffer.clear();
			indicesBuffer.clear();

			modelsOut.push_back(modelBuffer);
		}

		return modelsOut;
	}
}
