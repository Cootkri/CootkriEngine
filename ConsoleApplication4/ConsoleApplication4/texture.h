#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL/glew.h"

class Texture{
public:
	void Init(const std::string& fileName);
	Texture(){}
	
	void Bind(unsigned int unit);

	virtual ~Texture();

	std::string pathway;
	GLuint m_texture;
protected:
private:
	Texture(const Texture& other){}
	void operator =(const Texture& other){}
};

#endif