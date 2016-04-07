#include <iostream>
#include <math.h>
#include <cmath>
#include <windows.h>
#include "GL/glew.h"
#include "SDL2/SDL_mixer.h"

#include "display.h"
#include "shader.h"
#include "Mesh2.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "Wall.h"
#include "Player.h"
#include "World.h"
#include "Sprite.h"
#include "Ambient.h"
#include "Spotlight.h"
#include "Prop.h"
#include "Brush.h"

#define WIDTH		1040
#define HEIGHT		585

void OnClick(int type){
	
}

int main(int argc, char** argv){
	//ShowWindow(GetForegroundWindow(), SW_HIDE);
	Display display(WIDTH, HEIGHT, "OpenGL");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){ std::cout << "ERROR: MixOpenAudio: " << Mix_GetError() << std::endl; }

	World world; Shader* shader = world.GetShader();
	Brush br(24, 6, 24, &world, "./res/tex/pxl_cuboid.jpg"); br.Move(-2, 0, -2);

	Brush c1(1, 0.5, 3, &world, "./res/tex/pxl_cuboid.jpg"); c1.Move(2, 0, 2);
	Brush c2(1, 1.0, 3, &world, "./res/tex/pxl_cuboid.jpg"); c2.Move(3, 0, 2);
	Brush c3(1, 0.2, 1.5, &world, "./res/tex/pxl_cuboid.jpg"); c3.Move(6, 0.9, 2);
	Brush c4(1, 0.2, 1.5, &world, "./res/tex/pxl_cuboid.jpg"); c4.Move(6, 1.1, 3.5);

	Brush box(1, 1, 1, &world, "./res/tex/pxl_cuboid.jpg"); box.Move(6, 0.5, 6);

	Player player(shader); display.SetPlayer(player);
	Camera cammy; cammy.Init(glm::vec3(0, 0, 0), 70.0f, 1600 / 900, 0.01f, 1000.0f);
	Ambient amb1; shader->AddAmbient(&amb1); amb1.m_range = 3; amb1.m_intensity = 0.2;
	Ambient amb2; shader->AddAmbient(&amb2); amb2.SetPosition(1, 1, 1);
	Ambient amb3; shader->AddAmbient(&amb3); amb3.SetPosition(15, 1, 10); amb3.SetColor(0.2, 0.5, 0.7);

	Prop prop(world.GetMesh("./res/models/cucco.obj"), world.GetTexture("./res/models/cucco.jpg"), shader); world.AddProp(&prop);
	Prop stone(world.GetMesh("./res/models/salesman.obj"), world.GetTexture("./res/models/salesman.jpg"), shader); world.AddProp(&stone); stone.Scale(0.01); stone.Teleport(0, -10, 0);
	Sound laugh; laugh.Init("./res/sounds/sale_laugh.wav", 0.7); laugh.Load();
	
	player.SetListener(Player_OnClick, OnClick);

	float count = 0.0f;
	while (!display.IsClosed()){
		display.Clear(0.1f, 0.3f, 0.8f, 1.0f);
		world.Draw(&player.GetCamera());
		player.DrawHUD();
		display.Update();

		laugh.Update(player.GetCamera().GetPos());

		if (count >= 2 * PI){ count = 0; }
		count += 0.05f;
		c4.Rotate(0, 1, 0);

		if (glm::distance(prop.position, player.GetCamera().GetPos()) >= 2){
			prop.velocity = (player.GetCamera().GetPos() - prop.position) / 64.0f;
			prop.velocity.y = 0;
			prop.SetRotate(0, PI / 2 - GetAngleRad(glm::vec2(0, 0), GetXZ(prop.velocity)), 0);
		}
		else{
			prop.velocity *= 0;
		}
		prop.Rotate(0, sin(count) * PI / 360, 0);
		amb1.m_position = player.GetCamera().GetPos();

		if (player.mouse_buttons[0]){//Left Clicking
			player.mouse_buttons[0] = false;
			stone.Teleport(world.GetAimPoint(&player));
			stone.SetRotate(0, PI / 2 - GetAngleRad(glm::vec2(0, 0), GetXZ(player.GetCamera().GetPos() - stone.position)), 0);
			laugh.pos = stone.position;
			laugh.Emit(player.GetCamera().GetPos());
		}

		world.Update();
		player.Update();
		world.IsCollide(&player);
		player.CommenceVelocity();
	}
	Mix_CloseAudio();
	Mix_Quit();
	return 0;
}