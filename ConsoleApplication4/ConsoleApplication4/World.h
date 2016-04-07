#ifndef WORLD_H
#define WORLD_H

#include "Wall.h"
#include "camera.h"
#include "Player.h"
#include "Prop.h"
#include "GameFunctions.h"
#include "glm/glm.hpp"
#include <map>

#define MAX_WALLS	512
#define MAX_PROPS	128
#define COLL_BOUND	0.35f

class World{
public:
	World();
	~World();

	void AddWall(Wall* wall);
	void RemoveWall(Wall* wall);

	void AddProp(Prop* prop);
	void RemoveProp(Prop* prop);

	Mesh* GetMesh(std::string path);//Only caches if not already cached.
	Texture* GetTexture(std::string path);
	void ClearMesh(std::string path);//Erase and forget cached data
	void ClearTexture(std::string path);
	Shader* GetShader(){ return &shader; }
	void SetShader(std::string path){
		shader.Init(path);
	}

	void Update();
	void Draw(Camera* cam);
	void IsCollide(Player* player);
	glm::vec3 GetAimPoint(Player* player);
protected:
private:
	Wall* walls[MAX_WALLS];
	Prop* props[MAX_PROPS];
	Shader shader;
	unsigned int m_wallCount, m_propCount;
	std::map<std::string, Mesh> meshes;
	std::map<std::string, Texture> textures;
};

#endif