#include "Image.h"

Image::Image(){
	v.resize(4);
	in.resize(6);
}

void Image::Init(double xco, double yco, double width, double height, const char* tex, Shader* shader){
	v[0].Position = glm::vec3(xco, (yco + height), 0); v[0].TexCoords = glm::vec2(0, 0); v[0].Normal = glm::vec3();
	v[1].Position = glm::vec3(xco, yco, 0); v[1].TexCoords = glm::vec2(0, 1.0); v[1].Normal = glm::vec3();
	v[2].Position = glm::vec3((xco + width), yco, 0); v[2].TexCoords = glm::vec2(1.0, 1.0); v[2].Normal = glm::vec3();
	v[3].Position = glm::vec3((xco + width), (yco + height), 0); v[3].TexCoords = glm::vec2(1.0, 0); v[3].Normal = glm::vec3();
	in[0] = 0; in[1] = 1; in[2] = 2; in[3] = 2; in[4] = 3; in[5] = 0;

	m_mesh.Init(v, in);
	m_shader = shader;
	if (((std::string)tex).find(".png") != std::string::npos){ m_transparent = true; }else{ m_transparent = false; }
	m_texture.Init(tex);
}

Image::~Image(){
	m_mesh.~Mesh();
	m_texture.~Texture();
}

void Image::Draw(){
	if (m_transparent){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	m_shader->Bind();
	m_texture.Bind(0);
	m_shader->Update(m_transform);
	m_mesh.Draw();
	//glDisable(GL_BLEND);
}

void Image::DrawCam(Camera &cam){
	if (m_transparent){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	m_shader->Bind();
	m_texture.Bind(0);
	m_shader->Update(m_transform, cam);
	m_mesh.Draw();
}