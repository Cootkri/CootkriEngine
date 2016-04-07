#include "Player.h"
#include "glm/glm.hpp"
#include "GameFunctions.h"
#include <cmath>
#include <iostream>

#define WIDTH		1600
#define HEIGHT		900

const int Player::m_keys[MAX_KEYS] = { SDLK_a, SDLK_s, SDLK_d, SDLK_w, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_F11, SDLK_F10, SDLK_SPACE };

Player::Player(Shader* shade){
	shader = shade;
	spotlight.m_range = 10; shader->AddSpotlight(&spotlight); spotlight.m_intensity = 0;
	images[0].Init(-0.9f, 0.9f, 0.1f, 0.1f, "./res/heart.png", shader);
	images[1].Init(-0.9f, 0.8f, 0.1f, 0.1f, "./res/heart.png", shader);
	images[2].Init(-0.1f, -0.1f, 0.2f, 0.2f, "./res/crosshair.png", shader);
	flashlight.Init("./res/sounds/flashlight.wav", 1.0f); flashlight.Load();
	m_steps[0].Init("./res/sounds/footsteps/tile1.wav", 0.8f); m_steps[0].Load();
	m_steps[1].Init("./res/sounds/footsteps/tile2.wav", 0.8f); m_steps[1].Load();
	m_steps[2].Init("./res/sounds/footsteps/tile3.wav", 0.8f); m_steps[2].Load();
	m_steps[3].Init("./res/sounds/footsteps/jump.wav", 0.5f); m_steps[3].Load();
	memset(m_keysPress, false, MAX_KEYS);
	m_stepTicks[0] = 40; m_stepTicks[1] = 0;
	ticks = clock();
	m_height = 1.0f;
	m_onGround = false;
	mouse_buttons[0] = false; mouse_buttons[1] = false; mouse_buttons[2] = false;
	m_sensitivity = 20.0f;
	m_gravity = 0.007f;
	m_accel = 0.03f; m_accelCap = 0.07f;
	m_velocity = glm::vec3(0, 0, 0);
	m_friction = 0.015f; m_airres = 0.0005f;
	m_jumpBoost = 0.08;
	m_camera.Init(glm::vec3(0, (m_height + 0.05), 0), 70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	std::cout << "Player created" << std::endl;
}

void Player::Update(){
	if (m_stepTicks[1] >= m_stepTicks[0]){ m_stepTicks[1] = 0; m_steps[rand() % (int)(3)].Play(); }
	if (clock() > ticks + TICK){
		spotlight.m_pos = m_camera.GetPos(); spotlight.m_direction = m_camera.GetForward();
		if (m_stepTicks[1] > 0){ m_stepTicks[1]++; }
		CheckInput();
		double frict;
		if (!m_onGround){ m_velocity[1] -= m_gravity; frict = m_airres; }
		else{ m_velocity[1] = 0; frict = m_friction; }
		if (m_velocity[0] != 0 || m_velocity[2] != 0){//Apply friction
			double rot;
			if (m_velocity[2] == 0){ rot = (m_velocity[0] > 0 ? 0 : PI); }//Exception conditions incase 1 vector doesn't exist
			else if (m_velocity[0] == 0){
				rot = (m_velocity[2] > 0 ? (PI / 2.0) : (3.0 * PI / 2.0));
			}
			else{
				rot = atan(abs(m_velocity[2]) / abs(m_velocity[0]));//Always get positive angle as a triangle in first quadrant
				if (m_velocity[0] < 0 && m_velocity[2] < 0){ rot += PI; }
				if (m_velocity[0] < 0 && m_velocity[2] > 0){ rot = PI - rot; }
				if (m_velocity[0] > 0 && m_velocity[2] < 0){ rot = 2 * PI - rot ; }//Conditions to find angle of velocity
			}
			if (m_velocity[0] > 0){
				m_velocity[0] -= abs(cos(rot) * frict);
				if (m_velocity[0] < 0){ m_velocity[0] = 0; }
			}
			if (m_velocity[0] < 0){
				m_velocity[0] += abs(cos(rot) * frict);
				if (m_velocity[0] > 0){ m_velocity[0] = 0; }
			}
			if (m_velocity[2] > 0){
				m_velocity[2] -= abs(sin(rot) * frict);
				if (m_velocity[2] < 0){ m_velocity[2] = 0; }
			}
			if (m_velocity[2] < 0){
				m_velocity[2] += abs(sin(rot) * frict);
				if (m_velocity[2] > 0){ m_velocity[2] = 0; }
			}
			double resultant = sqrt(pow(m_velocity[0], 2) + pow(m_velocity[2], 2)), factor;//Total speed vector value
			if (resultant > m_accelCap){
				factor = resultant / m_accelCap;
				m_velocity[0] /= factor; m_velocity[2] /= factor;
			}
		}
	}
}

void Player::CommenceVelocity(){
	if (clock() > ticks + TICK){
		ticks = clock();
		Move(m_velocity[0], m_velocity[1], m_velocity[2]);
	}
}

void Player::DrawHUD(){
	GLuint bit = shader->effect;
	shader->SetEffect(1);
	images[0].Draw();
	images[1].Draw();
	images[2].Draw();
	shader->SetEffect(bit);
}

void Player::Teleport(double xco, double yco, double zco){
	m_camera.GetPos().x = (float)xco;
	m_camera.GetPos().y = (float)yco;
	m_camera.GetPos().z = (float)zco;
}

void Player::Teleport(glm::vec3 dest){
	m_camera.GetPos() = dest;
}

void Player::Move(double xco, double yco, double zco){
	if (xco == 0.0 && yco == 0.0 && zco == 0.0){ return; }
	m_camera.GetPos().x += (float)xco;
	m_camera.GetPos().y += (float)yco;
	m_camera.GetPos().z += (float)zco;
}

void Player::Walk(double fwd, double rl){
	if (fwd != 0){
		double rad = ((450 - m_camera.GetRot()) * PI / 180.0);
		m_velocity[0] += (sin(rad) * fwd);
		m_velocity[2] += (cos(rad) * fwd);
	}
	if (rl != 0){
		double rad = ((360 - m_camera.GetRot()) * PI / 180.0);
		m_velocity[0] += (sin(rad) * rl);
		m_velocity[2] += (cos(rad) * rl);
	}
}

void Player::OnPress(int key){
	for (std::map <void(*)(int), PlayerEvents>::iterator it = listeners.begin(); it != listeners.end(); it++){
		if (it->second == Player_OnKey){
			(it->first)(key);
		}
	}
}

void Player::OnRelease(int key){

}

void Player::CheckInput(){
	bool step = false; double newAccel;
	if (m_onGround){ newAccel = m_accel; }
	else{ newAccel = m_accel * 0.1; }
	for (unsigned int i = 0; i < MAX_KEYS; i++){//What to do for each key pressed
		if (!m_keysPress[i]){ continue; }
		switch (m_keys[i]){
			case SDLK_a:
				Walk(0, -newAccel); step = true; break;
			case SDLK_w:
				Walk(newAccel, 0); step = true; break;
			case SDLK_d:
				Walk(0, newAccel); step = true; break;
			case SDLK_s:
				Walk(-newAccel, 0); step = true; break;
			/*case SDLK_SPACE:
				if (m_onGround){ m_velocity[1] += m_jumpBoost; m_onGround = false; m_steps[3].Play(); std::cout << "Jump" << std::endl; } break;*/
		}
	}
	if (step && m_onGround){ if (m_stepTicks[1] == 0){ m_stepTicks[1] = 1; } }
	else{ m_stepTicks[1] = 0; }
}

void Player::KeyEvent(int key, bool keyDown){
	for (unsigned int i = 0; i < MAX_KEYS; i++){//Sets key bools & sends to events
		if (key == m_keys[i]){
			if (keyDown){
				if (!m_keysPress[i]){ OnPress(key); }
				m_keysPress[i] = true;
			}else{
				if (m_keysPress[i]){ OnRelease(key); }
				m_keysPress[i] = false;
			}
			break;
		}
	}
}

void Player::MMotionEvt(int xrel, int yrel){
	m_camera.Rotate(xrel / m_sensitivity);
	m_camera.Yaw(-yrel / m_sensitivity);
}

void Player::MouseClick(int x, int y, int button){//1=Left,2=Middle,3=Right
	if (button == 3){
		spotlight.m_intensity = 1 - spotlight.m_intensity;
		flashlight.Play();
	}
	if (button == 2){
		m_onGround = false;
		m_velocity[1] = 1;
	}
	if (button == 1){
		//Nothing yet
	}
	mouse_buttons[button - 1] = true;
	for (std::map <void(*)(int), PlayerEvents>::iterator it = listeners.begin(); it != listeners.end(); it++){
		if (it->second == Player_OnClick){
			(it->first)(button);
		}
	}
}

void Player::MouseRelease(int button){//1=Left,2=Middle,3=Right
	mouse_buttons[button - 1] = false;
}