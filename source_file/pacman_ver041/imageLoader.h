#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H
#include "global.h"
namespace texture_utils{
	void LoadTexture(const char *imageName, GLuint &texture);
	void SetTextureFiltering(GLuint texture, GLenum wrapS, GLenum wrapT, GLenum min, GLenum mag);
	void UpdateModelTexture(std::vector<Model> &models, GLuint textureIdx);
}
#endif	// end of IMAGE_LOADER_H
