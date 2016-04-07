#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "GL/glew.h"
#include "shader.h"
#include "transform.h"
#include "texture.h"
#include "Mesh2.h"

class Image{
public:
	Image();
	~Image();
	void Init(double xco, double yco, double width, double height, const char* texture, Shader* shader);
	void Draw(); void DrawCam(Camera &cam);
	void Teleport(double xco, double yco, double zco){
		m_transform.GetPos().x = (float)xco;
		m_transform.GetPos().y = (float)yco;
		m_transform.GetPos().z = (float)zco;
	}
	void Rotate(double xco, double yco, double zco){//IN RADIANS
		m_transform.GetRot().x = (float)xco;
		m_transform.GetRot().y = (float)yco;
		m_transform.GetRot().z = (float)zco;
		if (m_transform.GetRot().x > 2.0f * PI){ m_transform.GetRot().x -= 2.0f * PI; }
		if (m_transform.GetRot().y > 2.0f * PI){ m_transform.GetRot().y -= 2.0f * PI; }
		if (m_transform.GetRot().z > 2.0f * PI){ m_transform.GetRot().z -= 2.0f * PI; }
		if (m_transform.GetRot().x < -2.0f * PI){ m_transform.GetRot().x += 2.0f * PI; }
		if (m_transform.GetRot().y < -2.0f * PI){ m_transform.GetRot().y += 2.0f * PI; }
		if (m_transform.GetRot().z < -2.0f * PI){ m_transform.GetRot().z += 2.0f * PI; }
	}
protected:
private:
	std::vector<Vertex> v;
	std::vector<GLuint> in;
	Mesh m_mesh;
	Shader* m_shader;
	Transform m_transform;
	Texture m_texture;
	bool m_transparent;
};

#endif