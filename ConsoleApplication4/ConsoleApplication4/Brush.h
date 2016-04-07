#ifndef BRUSH_H
#define BRUSH_H

#include "Wall.h"
#include "World.h"

class Brush{
public:
	double position[3], dimensions[3];
	Texture* texture;

	Brush(){};
	Brush(double xd, double yd, double zd, World* wrld, const std::string tex, double texX=1, double texY=1);
	void Init(double xd, double yd, double zd, World* wrld, const std::string tex, double texX = 1, double texY = 1);
	~Brush(){
		for (glm::uint i = 0; i < 6; i++){
			world->RemoveWall(&walls[i]);
		}
	}

	void Move(double x, double y, double z);
	void Teleport(double x, double y, double z); void Teleport(glm::vec3 targ);
	void Rotate(double x, double y, double z);
protected:
private:
	World* world;
	Wall walls[6];
};

#endif