#pragma once

#include "stdafx.h"

#include <string.h>
#include <lodepng.h>

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string& filename);
	~Texture();

	bool load();

	void bind(GLenum textureUnit);
private:
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	unsigned char* image_data;
	size_t w, h;
};