#ifndef _GL_TEXTURE_H
#define _GL_TEXTURE_H

#include <string>

#include <opencv2/core.hpp>

#include <GL/glew.h>

/*The GLTexture class will load a texture image and 
* setup the OpenGL's texture unit to use this image.
*/
class GLTexture {
public:
	GLTexture(const std::string& aLoggerFileName,
			  const std::string& aImageFile, 
			  GLenum aTextureSlot);
	/* The GLTexture's object is not copyable and moveable
	*/
	GLTexture(const GLTexture&) = delete;
	GLTexture& operator=(const GLTexture&) = delete;
	GLTexture(GLTexture&&) = delete;
	GLTexture& operator=(GLTexture&&) = delete;
protected:
	/*
	* The utility function to load an RGBA image from file.
	*/
	bool load_image(const std::string& aLoggerFileName, const std::string& aImageFile);
private:
	cv::Mat m_texture_image;	// Store the image's pixel data
	int m_texture_image_width;	// The width of the image
	int m_texture_image_height;	// The height of the image
	unsigned int m_texture_id; // The OpenGL's texture object id
};


#endif
