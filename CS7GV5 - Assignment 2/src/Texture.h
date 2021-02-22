#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <cassert>
#include <iostream>

class Texture {
public:
	Texture(const std::string& filename);
	Texture(const std::vector<std::string>& filenames);
	virtual ~Texture();

	void Bind(unsigned int unit);

private:
	bool m_IsCubeMap = false;
	GLuint m_TextureID;
};
