#include "gl_texture.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "gl_logger.h"

#include <iostream>

GLTexture::GLTexture(const std::string& aLoggerFileName, 
					const std::string& aImageFile, 
					GLenum aTextureSlot):m_texture_id(0), 
										 m_texture_image_height(0), 
										 m_texture_image_width(0) {
	load_image(aLoggerFileName, aImageFile);
	std::cout << "Image width: " << m_texture_image_width << std::endl;
	std::cout << "Image height: " << m_texture_image_height << std::endl;
	std::cout << "Image channels: " << m_texture_image.channels() << std::endl;
	glGenTextures(1, &m_texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 
				0, 
				GL_RGBA, 
				m_texture_image_width, 
				m_texture_image_height, 
				0, 
				GL_RGBA, 
				GL_UNSIGNED_BYTE, 
				m_texture_image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

bool GLTexture::load_image(const std::string& aLoggerFileName, const std::string& aImageFile) {
	m_texture_image = cv::imread(aImageFile, cv::IMREAD_UNCHANGED);
	if (m_texture_image.empty()) {
		gl_log_err(aLoggerFileName.c_str(),
			"ERROR: Failed load the texture image: %s\n",
			aImageFile.c_str());
		return false;
	}
	else {
		// flip the image
		cv::flip(m_texture_image, m_texture_image, 0);
		// convert to RGBA
		cv::cvtColor(m_texture_image, m_texture_image, cv::COLOR_BGRA2RGBA);
		// Get the width and height
		cv::MatSize img_size = m_texture_image.size;
		m_texture_image_width = img_size[1];
		m_texture_image_height = img_size[0];
		return true;
	}
}