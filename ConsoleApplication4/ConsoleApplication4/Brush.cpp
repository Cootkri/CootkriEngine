#include "Brush.h"

Brush::Brush(double xd, double yd, double zd, World* wrld, const std::string tex, double texX, double texY){
	position[0] = 0; position[1] = 0; position[2] = 0;
	dimensions[0] = xd; dimensions[1] = yd; dimensions[2] = zd;
	world = wrld;
	float x = (float)xd; float y = (float)yd; float z = (float)zd;
	texture = wrld->GetTexture(tex);
	Shader* shade = wrld->GetShader();
	walls[0].Init(x, y, texture, shade, texX, texY); walls[0].Rotate(0, 0); (*world).AddWall(&walls[0]);
	walls[1].Init(x, z, texture, shade, texX, texY); walls[1].Rotate(90, 0); walls[1].Move(0, y, 0); (*world).AddWall(&walls[1]);
	walls[2].Init(z, y, texture, shade, texX, texY); walls[2].Rotate(-90, 1); (*world).AddWall(&walls[2]);
	walls[3].Init(z, y, texture, shade, texX, texY); walls[3].Rotate(-90, 1); walls[3].Move(x, 0, 0); (*world).AddWall(&walls[3]);
	walls[4].Init(x, y, texture, shade, texX, texY); walls[4].Move(0, 0, z); (*world).AddWall(&walls[4]);
	walls[5].Init(x, z, texture, shade, texX, texY); walls[5].Rotate(90, 0); (*world).AddWall(&walls[5]);
}

void Brush::Init(double xd, double yd, double zd, World* wrld, const std::string tex, double texX, double texY){
	position[0] = 0; position[1] = 0; position[2] = 0;
	dimensions[0] = xd; dimensions[1] = yd; dimensions[2] = zd;
	world = wrld;
	float x = (float)xd; float y = (float)yd; float z = (float)zd;
	texture = wrld->GetTexture(tex);
	Shader* shade = wrld->GetShader();
	walls[0].Init(x, y, texture, shade, texX, texY); walls[0].Rotate(0, 0); (*world).AddWall(&walls[0]);
	walls[1].Init(x, z, texture, shade, texX, texY); walls[1].Rotate(90, 0); walls[1].Move(0, y, 0); (*world).AddWall(&walls[1]);
	walls[2].Init(z, y, texture, shade, texX, texY); walls[2].Rotate(-90, 1); (*world).AddWall(&walls[2]);
	walls[3].Init(z, y, texture, shade, texX, texY); walls[3].Rotate(-90, 1); walls[3].Move(x, 0, 0); (*world).AddWall(&walls[3]);
	walls[4].Init(x, y, texture, shade, texX, texY); walls[4].Move(0, 0, z); (*world).AddWall(&walls[4]);
	walls[5].Init(x, z, texture, shade, texX, texY); walls[5].Rotate(90, 0); (*world).AddWall(&walls[5]);
}

void Brush::Move(double x, double y, double z){
	for (int i = 0; i < 6; i++){
		walls[i].Move((float)x, (float)y, (float)z);
	}
	position[0] += x; position[1] += y; position[2] += z;
}

void Brush::Teleport(double x, double y, double z){
	for (int i = 0; i < 6; i++){
		walls[i].Move((float)(x - position[0]), (float)(y - position[1]), (float)(z - position[2]));
	}
	position[0] = x; position[1] = y; position[2] = z;
}

void Brush::Teleport(glm::vec3 targ){
	for (int i = 0; i < 6; i++){
		walls[i].Move((float)(targ.x - position[0]), (float)(targ.y - position[1]), (float)(targ.z - position[2]));
	}
	position[0] = targ.x; position[1] = targ.y; position[2] = targ.z;
}

void Brush::Rotate(double xd, double yd, double zd){//In degrees; X Axis doesn't work completely
	float x = (float)xd; float y = (float)yd; float z = (float)zd;
	for (int i = 0; i < 6; i++){
			switch (i){
			case 1:{
					   glm::vec4 result = walls[0].transform.GetModel() * glm::vec4(0.0, dimensions[1], 0.0, 1.0);
					   walls[i].Teleport(result.x, result.y, result.z);
					   walls[i].Rotate(x, 0);
					   walls[i].Rotate(y, 1);
					   walls[i].Rotate(z, 2);
					   break;
			}
			case 3:{
					   glm::vec4 result = walls[0].transform.GetModel() * glm::vec4(dimensions[0], 0.0, 0.0, 1.0);
					   walls[i].Teleport(result.x, result.y, result.z);
					   walls[i].Rotate(x, 0);
					   walls[i].Rotate(y, 1);
					   walls[i].Rotate(z, 2);
					   break;
			}
			case 4:{
					   glm::vec4 result = walls[0].transform.GetModel() * glm::vec4(0.0, 0.0, dimensions[2], 1.0);
					   walls[i].Teleport(result.x, result.y, result.z);
					   walls[i].Rotate(x, 0);
					   walls[i].Rotate(y, 1);
					   walls[i].Rotate(z, 2);
					   break;
			}
			default:{
						walls[i].Rotate(x, 0);
						walls[i].Rotate(y, 1);
						walls[i].Rotate(z, 2);
			}
			}
	}
}