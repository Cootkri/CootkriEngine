#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <string>
#include "Player.h"

class Display{
public:
	Display(int width, int height, const std::string& title);
	
	void Clear(float r, float g, float b, float a);
	void Update();
	bool IsClosed();
	void SetPlayer(Player &plyr);
	void ToggleF();

	virtual ~Display();
protected:
private:
	Display(const Display& other){}
	void operator=(const Display& other){}
	Player *m_player;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed, m_fullscr;
	bool button_fscreen;
};

#endif