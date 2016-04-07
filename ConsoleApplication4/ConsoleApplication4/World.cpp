#include "World.h"
#include <vector>

World::World(){
	//memset(walls, 0, MAX_WALLS);
	for (unsigned int i = 0; i < MAX_WALLS; i++){ walls[i] = 0; }
	for (unsigned int i = 0; i < MAX_PROPS; i++){ props[i] = 0; }
	m_wallCount = 0; m_propCount = 0;
	SetShader("./res/shader2");
	std::cout << "World created" << std::endl;
}

glm::vec3 World::GetAimPoint(Player* player){
	glm::vec3 result = glm::vec3(0, 0, 0);
	glm::vec3 temp; double dist = 0, closest = 0;
	glm::vec3 origin = (*player).GetCamera().GetPos();
	glm::vec3 dir = (*player).GetCamera().GetForward();

	for (unsigned int i = 0; i < m_wallCount; i++){//Check all walls
		temp = RayShot(origin, dir, glm::vec2(walls[i]->lx, walls[i]->bx), glm::vec2(walls[i]->ly, walls[i]->by), glm::vec2(walls[i]->lz, walls[i]->bz));
		if (temp.x == 0.0f && temp.y == 0.0f && temp.z == 0.0f){ continue; }
		dist = glm::length(temp - origin);
		if (dist < closest || closest == 0){
			closest = dist;
			result = temp;
		}
	}
	for (unsigned int i = 0; i < m_propCount; i++){//Check all props
		temp = RayShot(origin, dir, glm::vec2(props[i]->smallvec.x, props[i]->bigvec.x), glm::vec2(props[i]->smallvec.y, props[i]->bigvec.y), glm::vec2(props[i]->smallvec.z, props[i]->bigvec.z));
		if (temp.x == 0.0f && temp.y == 0.0f && temp.z == 0.0f){ continue; }
		dist = glm::length(temp - origin);
		if (dist < closest || closest == 0){
			closest = dist;
			result = temp;
		}
	}

	return result;
}

void World::IsCollide(Player* player){
	if (clock() < player->ticks + TICK){ return; }
	glm::vec3 plpos, plvelo; glm::float32 pheight;
	double wawi, wahi;//Wall width & height
	plpos[0] = player->GetX(); plpos[1] = player->GetY(); plpos[2] = player->GetZ();
	pheight = player->GetHeight();
	plpos[1] -= pheight;
	plvelo[0] = player->m_velocity[0]; plvelo[1] = player->m_velocity[1]; plvelo[2] = player->m_velocity[2];
	bool fall = true;//Until loop says otherwise
	for (unsigned int i = 0; i < m_wallCount; i++){
		if (walls[i] == 0){ break; }
		wawi = walls[i]->GetWidth();
		wahi = walls[i]->GetHeight();

		double bx, lx, by, ly, bz, lz, slope = walls[i]->slope;
		bx = walls[i]->bx + COLL_BOUND; lx = walls[i]->lx - COLL_BOUND;
		by = walls[i]->by; ly = walls[i]->ly;
		bz = walls[i]->bz + COLL_BOUND; lz = walls[i]->lz - COLL_BOUND;

		if (plpos[1] < by && plpos[1] + pheight > ly){//Touching on Y-axis
			if ((plpos[0] > bx && plpos[0] + plvelo[0] < bx) || (plpos[0] < lx && plpos[0] + plvelo[0] > lx)){//Past an X pseudo-wall
				if (slope == 0 && plpos[2] < bz && plpos[2] > lz){//Flat wall check
					plvelo[0] = 0;
				}
			}
			if ((plpos[2] > bz && plpos[2] + plvelo[2] < bz) || (plpos[2] < lz && plpos[2] + plvelo[2] > lz)){//Past a Z pseudo-wall
				if (slope == 0 && plpos[0] < bx && plpos[0] > lx){//Flat wall check
					plvelo[2] = 0;
				}
			}
			if (slope != 0 && plpos[0] < bx && plpos[0] > lx && plpos[2] < bz && plpos[2] > lz){//If already past pseudo-walls, check slopes
				double zpoint, zpoint2;
				if (slope > 0){ zpoint = lz + (plpos[0] - lx) * slope, zpoint2 = lz + (plpos[0] + plvelo[0] - lx) * slope; }
				else{ zpoint = bz + (plpos[0] - lx) * slope, zpoint2 = bz + (plpos[0] + plvelo[0] - lx) * slope; }
				if ((plpos[2] + COLL_BOUND > zpoint && plpos[2] + plvelo[2] - COLL_BOUND < zpoint2) ||
					(plpos[2] - COLL_BOUND < zpoint && plpos[2] + plvelo[2] + COLL_BOUND > zpoint2)){
					plvelo[2] = 0;
					plvelo[0] = 0;
				}
			}
		}
		if (plpos[1] > by && plvelo[1] + plpos[1] < by){//About to pass beneath Y-Axis
			if (plpos[0] < bx && plpos[0] > lx && plpos[2] < bz && plpos[2] > lz){ plvelo[1] = by - plpos[1]; fall = false; }
		}
		else if (plpos[1] + pheight < ly && plpos[1] + pheight + plvelo[1] > ly){//About to pass above Y-Axis
			if (plpos[0] < bx && plpos[0] > lx && plpos[2] < bz && plpos[2] > lz){ plvelo[1] = ly - plpos[1] - pheight - 0.04; }
		}
		if ((plpos[1] < by + 0.005 && plpos[1] > by - 0.005) && plpos[0] < bx && plpos[0] > lx && plpos[2] < bz && plpos[2] > lz){ fall = false; }
	}
	player->m_onGround = !fall;
	player->m_velocity = plvelo;
}

/*void World::IsCollide(Player* player){
	if (clock() < player->ticks + TICK){ return; }
	glm::float32 pheight;
	glm::vec3 plpos, plvelo, pldest, bv, sv, vertices[4], normal;
	pheight = player->GetHeight();
	plpos = player->GetCamera().GetPos();
	plpos.y -= pheight;
	plvelo = player->m_velocity;
	plvelo.y -= 0.007f;
	pldest = plpos + plvelo;
	bool fall = true, changed = false;//Until loop says otherwise
	for (GLuint i = 0; i < m_wallCount; i++){
		if (walls[i] == 0){ break; }
		bv = walls[i]->bigvec; sv = walls[i]->smallvec;
		vertices[0] = walls[i]->vertices[0];
		vertices[1] = walls[i]->vertices[1];
		vertices[2] = walls[i]->vertices[2];
		vertices[3] = walls[i]->vertices[3];

		bool check = true;
		for (GLuint u = 0; u<2; u++){//X & Z
			if (u == 1){ u = 2; }
			if (!((pldest[u] + COLL_BOUND > sv[u] && pldest[u] - COLL_BOUND < bv[u]) || (plpos[u] + COLL_BOUND > sv[u] && plpos[u] - COLL_BOUND < bv[u]) ||
				(plpos[u] - COLL_BOUND < sv[u] && pldest[u] + COLL_BOUND > sv[u]) || (plpos[u] + COLL_BOUND > bv[u] && pldest[u] - COLL_BOUND < bv[u]))){
				check = false;
				break;
			}
		}
		if (check && !((pldest.y + pheight > sv.y && pldest.y < bv.y) || (plpos.y + pheight > sv.y && plpos.y < bv.y) ||
			(plpos.y < sv.y && pldest.y + pheight > sv.y) || (plpos.y + pheight > bv.y && pldest.y < bv.y))){
			check = false;
		}
		if (!check){//Skip this wall if impossible to touch
			continue;
		}

		normal = walls[i]->normal;
		glm::float32 temp = glm::dot(plvelo, normal);
		//glm::vec3 temp2 = normalize(temp*normal)*COLL_BOUND; temp2 *= 0;
		glm::vec3 temp2 = normal*COLL_BOUND;
		glm::vec3 intersect = PlaneIntersection(plvelo, plpos, normal, vertices[0]);
		glm::float32 offset = glm::length(intersect - plpos);
		if (offset <= glm::length(plvelo) + COLL_BOUND){
			//if (IsCoplanarPointOnFace(intersect, vertices[0], vertices[1], vertices[3])){
			if (offset <= COLL_BOUND ||
				IsPointInPrism(intersect, vertices[0] + temp2, vertices[0] - temp2, vertices[1] + temp2, vertices[3] + temp2)){
				glm::vec3 v1 = temp*normal;
				plvelo -= v1;
				if (v1.y < 0){
					player->normalForce = -v1.y / 0.07f;
					changed = true;
				}
				//std::cout << "DO TOUCH" << std::endl;
			}
			else{
				//std::cout << "NOT TOUCH" << std::endl;
			}
		}
		else{
			//std::cout << "NOT LONG ENOUGH" << std::endl;
			//std::cout << "NOT LONG ENOUGH: " << offset << " > " << glm::length(plvelo) << std::endl;
		}
	}
	if (!changed){ player->normalForce = 1.0f; }
	player->m_onGround = false;
	player->m_velocity = plvelo;
}*/

/*void World::IsCollide(Player* player){
	if (clock() < player->ticks + TICK){ return; }
	glm::float32 pheight;
	glm::vec3 plpos, plvelo, pldest, bv, sv, vertices[4], normal;
	pheight = player->GetHeight();
	plpos = player->GetCamera().GetPos();
	plpos.y -= pheight;
	plvelo = player->m_velocity;
	plvelo.y -= 0.007f;
	pldest = plpos + plvelo;
	bool fall = true, changed = false;//Until loop says otherwise
	std::vector<int> wallnum(m_wallCount); std::vector<float> wallval(m_wallCount);
	int wallcheck = 0;
	for (GLuint i = 0; i < m_wallCount; i++){
		if (walls[i] == 0){ break; }
		bv = walls[i]->bigvec; sv = walls[i]->smallvec;
		vertices[0] = walls[i]->vertices[0];
		vertices[1] = walls[i]->vertices[1];
		vertices[2] = walls[i]->vertices[2];
		vertices[3] = walls[i]->vertices[3];

		bool check = true;
		for (GLuint u = 0; u<2; u++){//X & Z
			if (u == 1){ u = 2; }
			if (!((pldest[u] + COLL_BOUND > sv[u] && pldest[u] - COLL_BOUND < bv[u]) || (plpos[u] + COLL_BOUND > sv[u] && plpos[u] - COLL_BOUND < bv[u]) ||
				(plpos[u] - COLL_BOUND < sv[u] && pldest[u] + COLL_BOUND > sv[u]) || (plpos[u] + COLL_BOUND > bv[u] && pldest[u] - COLL_BOUND < bv[u]))){
				check = false;
				break;
			}
		}
		if (check && !((pldest.y + pheight > sv.y && pldest.y < bv.y) || (plpos.y + pheight > sv.y && plpos.y < bv.y) ||
			(plpos.y < sv.y && pldest.y + pheight > sv.y) || (plpos.y + pheight > bv.y && pldest.y < bv.y))){
			check = false;
		}
		if (check){//Skip this wall if impossible to touch
			wallnum[wallcheck] = i;

			normal = walls[i]->normal;
			glm::vec3 intersect = PlaneIntersection(plvelo, plpos, normal, vertices[0]);

			wallval[wallcheck] = glm::length(intersect - plpos);
			wallcheck++;
		}
	}
	for (GLuint i = 0; i < wallcheck;i++){
		GLuint lowestWall = NULL, index;
		for (GLuint u = 0; u < wallval.size(); u++){
			if (lowestWall == NULL || wallval[u] < wallval[lowestWall]){
				lowestWall = u;
			}
		}
		wallval.erase(wallval.begin() + lowestWall);
		index = wallnum[lowestWall];
		wallnum.erase(wallnum.begin() + lowestWall);

		vertices[0] = walls[index]->vertices[0];
		vertices[1] = walls[index]->vertices[1];
		vertices[2] = walls[index]->vertices[2];
		vertices[3] = walls[index]->vertices[3];

		normal = walls[index]->normal;
		glm::float32 temp = glm::dot(plvelo, normal);
		glm::vec3 temp2 = normal*COLL_BOUND;
		glm::vec3 intersect = PlaneIntersection(plvelo, plpos, normal, vertices[0]);
		glm::float32 offset = glm::length(intersect - plpos);
		if (offset <= glm::length(plvelo) + 2*COLL_BOUND){
			if (IsPointInPrism(intersect, vertices[0] + temp2, vertices[0] - temp2, vertices[1] + temp2, vertices[3] + temp2)){
				glm::vec3 v1 = temp*normal;
				plvelo -= v1;
				if (v1.y < 0){
					player->normalForce = -v1.y / 0.07f;
					changed = true;
				}
			}
		}
	}
	if (!changed){ player->normalForce = 0.2f; }
	player->m_onGround = false;
	player->m_velocity = plvelo;
}*/

Mesh* World::GetMesh(std::string path){
	if (meshes.find(path) == meshes.end()){
		meshes[path].Init(path);
	}
	return &meshes[path];
}

Texture* World::GetTexture(std::string path){
	if (textures.find(path) == textures.end()){
		textures[path].Init(path);
	}
	return &textures[path];
}

void World::ClearMesh(std::string path){
	if (meshes.find(path) == meshes.end()){ return; }
	meshes.erase(path);
}

void World::ClearTexture(std::string path){
	if (textures.find(path) == textures.end()){ return; }
	textures.erase(path);
}

void World::Update(){
	for (unsigned int i = 0; i < m_propCount; i++){
		if (props[i] == 0){ break; }
		props[i]->Update();
	}
}

void World::AddWall(Wall* wall){
	walls[m_wallCount] = wall;
	m_wallCount++;
}

void World::RemoveWall(Wall* wall){
	bool found = false;
	for (unsigned int i = 0; i < m_wallCount; i++){
		if (walls[i] == wall){
			found = true;
			walls[i] = 0;
			continue;
		}
		if (found){
			if (walls[i] != 0){
				walls[i - 1] = walls[i];
				walls[i] = 0;
			}
			else{
				break;
			}
		}
	}
	if (found){
		m_wallCount--;
	}
}

void World::AddProp(Prop* prop){
	props[m_propCount] = prop;
	prop->Update();
	m_propCount++;
}

void World::RemoveProp(Prop* prop){
	bool found = false;
	for (unsigned int i = 0; i < m_propCount; i++){
		if (props[i] == prop){
			found = true;
			props[i] = 0;
			continue;
		}
		if (found){
			if (props[i] != 0){
				props[i - 1] = props[i];
				props[i] = 0;
			}
			else{
				break;
			}
		}
	}
	if (found){
		m_propCount--;
	}
}

void World::Draw(Camera* cam){
	glm::vec2 origin = glm::vec2(cam->GetPos().x, cam->GetPos().z);
	double view = GetAngleRad(glm::vec2(0,0), glm::vec2(cam->GetForward().x, cam->GetForward().z));
	double ang1, ang2;

	for (unsigned int i = 0; i < m_wallCount; i++){
		if (walls[i] == 0){ break; }
		if (walls[i]->IsTransparent()){ continue;  }
		ang1 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->bx, walls[i]->bz)));
		ang2 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->lx, walls[i]->lz)));
		if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
			walls[i]->Draw(cam);
		}
		else{
			ang1 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->bx, walls[i]->lz)));
			ang2 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->lx, walls[i]->bz)));
			if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
				walls[i]->Draw(cam);
			}
		}
	}
	for (unsigned int i = 0; i < m_propCount; i++){
		if (props[i] == 0){ break; }
		if (props[i]->isTransparent){ continue; }
		ang1 = abs(view - GetAngleRad(origin, glm::vec2(props[i]->bigvec.x, props[i]->bigvec.z)));
		ang2 = abs(view - GetAngleRad(origin, glm::vec2(props[i]->smallvec.x, props[i]->smallvec.z)));
		if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
			props[i]->Draw(cam);
		}
	}


	for (unsigned int i = 0; i < m_wallCount; i++){//Draw transparents last
		if (walls[i] == 0){ break; }
		if (walls[i]->IsTransparent()){
			ang1 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->bx, walls[i]->bz)));
			ang2 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->lx, walls[i]->lz)));
			if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
				walls[i]->Draw(cam);
			}
			else{
				ang1 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->bx, walls[i]->lz)));
				ang2 = abs(view - GetAngleRad(origin, glm::vec2(walls[i]->lx, walls[i]->bz)));
				if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
					walls[i]->Draw(cam);
				}
			}
		}
	}
	for (unsigned int i = 0; i < m_propCount; i++){
		if (props[i] == 0){ break; }
		if (props[i]->isTransparent){
			ang1 = abs(view - GetAngleRad(origin, glm::vec2(props[i]->bigvec.x, props[i]->bigvec.z)));
			ang2 = abs(view - GetAngleRad(origin, glm::vec2(props[i]->smallvec.x, props[i]->smallvec.z)));
			if (ang1 <= M_PI / 2 || ang1 >= M_PI * 3 / 2 || ang2 <= M_PI / 2 || ang2 >= M_PI * 3 / 2){//Only draw if visible
				props[i]->Draw(cam);
			}
		}
	}
}

World::~World(){
	meshes.erase(meshes.begin(), meshes.end());
	textures.erase(textures.begin(), textures.end());
}