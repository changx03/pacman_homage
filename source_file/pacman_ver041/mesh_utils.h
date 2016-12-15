#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include "shader.h"	// global.h already included in this header

namespace mesh_utils{
	void PrintSceneInfro(const aiScene *mScene);
	void LoadMeshVertices(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &vertices);
	void LoadMaterialColor(const aiScene *scene, size_t meshIdx, glm::vec3 &color);
	void LoadMeshNormals(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &normals);
	void LoadMeshIdx(const aiScene *scene, size_t meshIdx, std::vector<GLuint> &indices);
	std::vector<Model> LoadModels(Shader shader, const aiScene *scene, bool isColor = true, bool isTexture = false);
	void LoadMeshTextureCoord(const aiScene *scene, size_t meshIdx, std::vector<GLfloat> &textureCoords);
}
#endif	// end of mesh_utils_h
