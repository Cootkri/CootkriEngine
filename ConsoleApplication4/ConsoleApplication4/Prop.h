#ifndef PROP_H
#define PROP_H

#include <time.h>

//#include "mesh.h"
#include"Mesh2.h"
#include "texture.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"

class Prop{
public:
	glm::vec3 velocity, rotation, position;
	bool isTransparent;

	Prop(Mesh* model, Texture* texture, Shader* shader);
	void Init(Mesh* model, Texture* texture, Shader* shader);
	Prop(){}
	~Prop(){}

	void Draw(Camera *cam);
	void Update();
	void Teleport(double x, double y, double z);
	void Teleport(glm::vec3 targ);
	void Move(double x, double y, double z);
	void Move(glm::vec3 targ);
	void Rotate(double radx, double rady, double radz);
	void SetRotate(double radx, double rady, double radz);
	void Scale(double factor);

	glm::vec3 smallvec, bigvec;
protected:
private:
	Mesh* mesh;
	Shader* shader;
	Transform transform;
	Texture* tex;
	clock_t ticks;
};

#endif