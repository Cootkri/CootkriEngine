#ifndef WALL_H
#define WALL_H

#include "Mesh2.h"
#include "GL/glew.h"
#include <vector>
#include <string>
#include "shader.h"
#include "transform.h"
#include "texture.h"
#include "display.h"
//#include "mesh.h"

class Wall{
public:
	Transform transform;

	Wall(){}
	void Init( float w, float h, Texture* tex, Shader* shade, double texX = 1, double texY = 1){//Triangle left to right faces positive, right to left faces negative
		texture = tex;
		shader = shade;
		width = w; height = h; slope = 0;
		m_draw = true;
		coords[0] = 0; coords[1] = 0; coords[2] = 0;
		rotation[0] = 0; rotation[1] = 0; rotation[2] = 0;

		texX = w / texX;
		texY = h / texY;

		v.resize(4); in.resize(12);

		v[0].Position = glm::vec3(0, height, 0); v[0].TexCoords = glm::vec2(0.0, texY);
		v[1].Position = glm::vec3(0, 0, 0); v[1].TexCoords = glm::vec2(0.0, 0);
		v[2].Position = glm::vec3(width, 0, 0); v[2].TexCoords = glm::vec2(texX, 0);
		v[3].Position = glm::vec3(width, height, 0); v[3].TexCoords = glm::vec2(texX, texY);
		in[0] = 0; in[1] = 1; in[2] = 2; in[3] = 2; in[4] = 3; in[5] = 0;
		in[6] = 2; in[7] = 1; in[8] = 0; in[9] = 0; in[10] = 3; in[11] = 2;
		glm::vec3 nrml = glm::cross((v[1].Position - v[0].Position), (v[2].Position - v[0].Position));
		v[0].Normal = nrml; v[1].Normal = nrml; v[2].Normal = nrml; v[3].Normal = nrml;
		normal = nrml;

		mesh.Init(v, in);

		if ((texture->pathway).find(".png") != std::string::npos){ m_transparent = true; }
		else{ m_transparent = false; }
		Translate();
	}
	void Draw(Camera* cam){
		if (!m_draw){ return; }
		if (m_transparent){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		shader->Bind();
		texture->Bind(0);
		shader->Update(transform, *cam);
		mesh.Draw();
	}
	void Teleport(float xco, float yco = 0, float zco = 0){
		transform.GetPos().x = xco; coords[0] = xco;
		transform.GetPos().y = yco; coords[1] = yco;
		transform.GetPos().z = zco; coords[2] = zco;
		Translate();
	}
	void Move(float xco, float yco = 0, float zco = 0){
		transform.GetPos().x += xco; coords[0] += xco;
		transform.GetPos().y += yco; coords[1] += yco;
		transform.GetPos().z += zco; coords[2] += zco;
		Translate();
	}
	void Rotate(float deg, int axis){//0:X, 1:Y, 2:Z
		deg = (float)(deg * PI / 180.0);
		rotation[axis] += deg;
		switch (axis){
		case 0:
			transform.GetRot().x += deg;
			if (transform.GetRot().x > 2 * PI){ transform.GetRot().x -= 2 * PI; }
			if (transform.GetRot().x < -2 * PI){ transform.GetRot().x += 2 * PI; }
			break;
		case 1:
			transform.GetRot().y += deg;
			if (transform.GetRot().y > 2 * PI){ transform.GetRot().y -= 2 * PI; }
			if (transform.GetRot().y < -2 * PI){ transform.GetRot().y += 2 * PI; }
			break;
		case 2:
			transform.GetRot().z += deg;
			if (transform.GetRot().z > 2 * PI){ transform.GetRot().z -= 2 * PI; }
			if (transform.GetRot().z < -2 * PI){ transform.GetRot().z += 2 * PI; }
			break;
		}
		Translate();
	}

	double bx, lx, by, ly, bz, lz;
	double slope;
	glm::vec3 bigvec, smallvec, normal;
	glm::vec3 vertices[4];

	inline double GetWidth(){ return width; }
	inline double GetHeight(){ return height; }
	inline double GetX(){ return coords[0]; }
	inline double GetY(){ return coords[1]; }
	inline double GetZ(){ return coords[2]; }
	inline double GetXRot(){ return rotation[0]; }
	inline double GetYRot(){ return rotation[1]; }
	inline double GetZRot(){ return rotation[2]; }
	inline bool IsTransparent(){ return m_transparent; }
	void Translate(){
		glm::vec4 vect[4] = { { -0.04f, -0.04f, 0.0f, 1.0f }, { width + 0.04f, -0.04f, 0.0f, 1.0f }, { width + 0.04f, height + 0.04f, 0.0f, 1.0f }, { -0.04f, height + 0.04f, 0.0f, 1.0f } };
		int vecIds[2] = { 0, 0 };
		for (unsigned int u = 0; u<4; u++){//Get all four corners starting from center to counter-clockwise
			vect[u] = transform.GetModel() * vect[u];
			vertices[u] = glm::vec3(vect[u]);
			if (vect[u].x > -0.01 && vect[u].x < 0.01){ vect[u].x = 0.0f; }
			if (vect[u].y > -0.01 && vect[u].y < 0.01){ vect[u].y = 0.0f; }
			if (vect[u].z > -0.01 && vect[u].z < 0.01){ vect[u].z = 0.0f; }
			if (u == 0){ bx = vect[u].x; lx = bx; by = vect[u].y; ly = by; bz = vect[u].z; lz = bz; }
			if (vect[u].x > bx){ bx = vect[u].x; vecIds[0] = u; }
			else if (vect[u].x < lx){ lx = vect[u].x; vecIds[1] = u; }
			if (vect[u].y > by){ by = vect[u].y; }
			else if (vect[u].y < ly){ ly = vect[u].y; }
			if (vect[u].z > bz){ bz = vect[u].z; }
			else if (vect[u].z < lz){ lz = vect[u].z; }
		}
		if (bx + 0.005 > lx && bx - 0.005 < lx){ slope = 0; }//Because the numbers are never EXACTLY equivalent
		else{
			slope = (vect[vecIds[0]].z - vect[vecIds[1]].z) / (bx - lx);//Change in Z per change in X
		}
		bigvec = glm::vec3(bx, by, bz); smallvec = glm::vec3(lx, ly, lz);
		normal = normalize(glm::cross((vertices[1] - vertices[0]), (vertices[2] - vertices[1])));
	}
protected:
private:
	
	Mesh mesh;
	Shader* shader;
	Texture* texture;
	Transform blank;
	std::vector<Vertex> v;
	std::vector<GLuint> in;
	float coords[3], rotation[3];
	float width, height;
	bool m_draw, m_transparent;
};

#endif