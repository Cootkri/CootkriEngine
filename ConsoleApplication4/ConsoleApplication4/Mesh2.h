#ifndef MESH2_H
#define MESH2_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "obj_loader.h"
#include <iostream>
#include <vector>

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Tex{
	GLuint id;
	std::string type;
};

class Mesh{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	//std::vector<Tex> textures;

	Mesh(){}

	//void Init(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Tex> textures);
	void Init(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	void Init(const std::string& fileName);

	void Draw();

	glm::vec3 smallvec, bigvec;
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
	void SetBounds();
};

#endif