#include "imageLoader.h"

namespace texture_utils{
	void LoadTexture(const char *imageName, GLuint &texture){
		/* generate & bind texture objects*/
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		/* load image data */
		int width, height;	// image pixel size
		unsigned char *image = SOIL_load_image(imageName, &width, &height, 0, SOIL_LOAD_RGB);
		/* copy image data to texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);	// delete image data

		glBindTexture(GL_TEXTURE_2D, 0);	// unbind texture when done

		//gl_utils::GlErrorCheck(true);
	}

	void SetTextureFiltering(GLuint texture, GLenum wrapS, GLenum wrapT, GLenum min, GLenum mag){
		glBindTexture(GL_TEXTURE_2D, texture);
		/* texture wrapping mode */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		/* texture filtering */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);	// unbind texture when done
	}

	void UpdateModelTexture(std::vector<Model> &models, GLuint textureIdx){
		const size_t numModels = models.size();
		for (size_t i = 0; i < numModels; i++){
			models[i].Set_texture(textureIdx);
		}
	}
}
