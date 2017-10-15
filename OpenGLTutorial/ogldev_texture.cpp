#include "stdafx.h"
#include "ogldev_texture.h"

Texture::Texture(GLenum textureTarget, const std::string & filename)
{
	m_textureTarget = textureTarget;
	m_filename = filename;
}

Texture::~Texture()
{
	free(image_data);
}

bool Texture::load()
{
	unsigned int error = lodepng_decode32_file(&image_data,&w,&h,m_filename.c_str());
	if (error) {
		printf("Error loading texture: %s\n", lodepng_error_text(error));
		return false;
	}
	
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
