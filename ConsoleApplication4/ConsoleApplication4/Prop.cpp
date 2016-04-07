#include "Prop.h"

#define TICK	10//Milliseconds

Prop::Prop(Mesh* model, Texture* texture, Shader* shade){
	mesh = model;
	tex = texture;
	shader = shade;
	isTransparent = (tex->pathway.find(".png") != std::string::npos);
	transform.SetPos(glm::vec3(0, 0, 0));
	transform.GetScale().x = 0.25; transform.GetScale().y = 0.25; transform.GetScale().z = 0.25;
	velocity[0] = 0; velocity[1] = 0; velocity[2] = 0;
	rotation[0] = 0; rotation[1] = 0; rotation[2] = 0;
	position[0] = 0; position[1] = 0; position[2] = 0;
	smallvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->smallvec, 1.0));
	bigvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->bigvec, 1.0));
}

void Prop::Init(Mesh* model, Texture* texture, Shader* shade){
	mesh = model;
	tex = texture;
	shader = shade;
	isTransparent = (tex->pathway.find(".png") != std::string::npos);
	transform.SetPos(glm::vec3(0, 0, 0));
	transform.GetScale().x = 0.25; transform.GetScale().y = 0.25; transform.GetScale().z = 0.25;
	velocity[0] = 0; velocity[1] = 0; velocity[2] = 0;
	rotation[0] = 0; rotation[1] = 0; rotation[2] = 0;
	position[0] = 0; position[1] = 0; position[2] = 0;
	smallvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->smallvec, 1.0));
	bigvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->bigvec, 1.0));
}

void Prop::Draw(Camera *cam){
	shader->Bind();
	tex->Bind(0);
	//glDepthFunc(GL_ALWAYS);
	shader->Update(transform, *cam);
	//shader.SetFBright(1);
	mesh->Draw();
	//glDepthFunc(GL_LESS);
}

void Prop::Update(){
	if (clock() > ticks + TICK){
		if (glm::length(velocity) > 0){
			Move(velocity);
		}
		smallvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->smallvec, 1.0));
		bigvec = glm::vec3(transform.GetModel() * glm::vec4(mesh->bigvec, 1.0));

		ticks = clock();
	}
}

void Prop::Teleport(double x, double y, double z){
	glm::vec3 pos = glm::vec3(x, y, z);
	transform.SetPos(pos);
	position = pos;
}

void Prop::Teleport(glm::vec3 targ){
	transform.SetPos(targ);
	position = targ;
}

void Prop::Move(double x, double y, double z){
	glm::vec3 pos = glm::vec3(x, y, z);
	transform.GetPos() += pos;
	position += pos;
}

void Prop::Move(glm::vec3 targ){
	transform.GetPos() += targ;
	position += targ;
}

void Prop::Rotate(double x, double y, double z){
	rotation += glm::vec3(x, y, z) * 180.0f / PI;
	if (rotation[0] >= 360){ rotation[0] -= 360; }
	if (rotation[0] <= -360){ rotation[0] += 360; }
	if (rotation[1] >= 360){ rotation[1] -= 360; }
	if (rotation[1] <= -360){ rotation[1] += 360; }
	if (rotation[2] >= 360){ rotation[2] -= 360; }
	if (rotation[2] <= -360){ rotation[2] += 360; }

	transform.GetRot() += glm::vec3(x, y, z);
}

void Prop::SetRotate(double x, double y, double z){
	rotation = glm::vec3(x, y, z);
	transform.GetRot() = rotation;
}

void Prop::Scale(double factor){
	float fl = (float)factor;
	transform.GetScale() = glm::vec3(fl, fl, fl);
}