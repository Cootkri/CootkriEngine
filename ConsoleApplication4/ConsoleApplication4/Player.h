#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>
#include <SDL2/SDL.h>
#include "camera.h"
#include "Sound.h"
#include "Image.h"
#include <map>

#define TICK		10//10 milliseconds
#define MAX_KEYS	11//Amount of keys that have input

enum PlayerEvents{
	Player_NONE,
	Player_OnClick,
	Player_OnKey
};

class Player{
public:
	Player(Shader* shader);
	glm::vec3 m_velocity;
	glm::float32 normalForce;
	bool m_onGround, mouse_buttons[3];
	clock_t ticks; Shader* shader;

	void Teleport(double xco, double yco, double zco); void Teleport(glm::vec3 dest);
	void Move(double xco, double yco = 0, double zco = 0);
	void Walk(double forward, double rightleft);
	void KeyEvent(int key, bool keyDown);
	void MMotionEvt(int xrel, int yrel);
	void MouseClick(int x, int y, int button);
	void MouseRelease(int button);

	void Update();
	void CommenceVelocity();
	void DrawHUD();

	Camera& GetCamera(){ return m_camera; }
	inline double GetHeight(){ return m_height; }
	inline double GetX(){ return m_camera.GetPos().x; }
	inline double GetY(){ return m_camera.GetPos().y; }
	inline double GetZ(){ return m_camera.GetPos().z; }

	void SetListener(PlayerEvents type, void(*f)(int)){ listeners[f] = type; }
	void RemoveListener(void(*f)(int)){ listeners.erase(f); }
protected:
private:
	void CheckInput();
	void OnPress(int key), OnRelease(int key);
	static const int m_keys[MAX_KEYS];
	bool m_keysPress[MAX_KEYS];

	Camera m_camera; SDL_Event m_evt;
	double m_height, m_sensitivity, m_gravity, m_jumpBoost;
	double m_accel, m_accelCap, m_friction, m_airres;
	Image images[3];
	Spotlight spotlight;

	unsigned int m_stepTicks[2]; Sound m_steps[4]; Sound flashlight;
	std::map<void(*)(int), PlayerEvents> listeners;
};

#endif