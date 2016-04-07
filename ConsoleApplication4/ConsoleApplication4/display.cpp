#include "display.h"
#include <iostream>
#include "GL/glew.h"

Display::Display(int width, int height, const std::string& title){
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_DisplayMode dmode;
	SDL_GetCurrentDisplayMode(0, &dmode);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(dmode.w * 0.9), (int)(dmode.h * 0.9), SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();
	if (status != GLEW_OK){
		std::cerr << "GLEW FAILED TO INITIALIZE" << std::endl;
	}
	m_isClosed = false;
	m_fullscr = false; button_fscreen = false;
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	std::cout << "Display created" << std::endl;
}

Display::~Display(){
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed(){
	return m_isClosed;
}

void Display::SetPlayer(Player &plyr){
	m_player = &plyr;
}

void Display::ToggleF(){
	if (!m_fullscr){
		m_fullscr = true;
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	}
	else{
		m_fullscr = false;
		SDL_SetWindowFullscreen(m_window, 0);
	}
}

void Display::Update(){
	SDL_GL_SwapWindow(m_window);
	SDL_Event e;
	while (SDL_PollEvent(&e)){
		if(e.type == SDL_KEYDOWN){
			SDL_Keycode key = e.key.keysym.sym;
			if (key == SDLK_F11 && !button_fscreen){ ToggleF(); button_fscreen = true; }
			if (key == SDLK_ESCAPE){
				//m_isClosed = true;
				//SDL_Quit();
				if (SDL_GetRelativeMouseMode() == SDL_TRUE){
					SDL_ShowCursor(1);
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				else{
					SDL_ShowCursor(0);
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
			(*m_player).KeyEvent(key, true);
		}
		if (e.type == SDL_KEYUP){
			SDL_Keycode key = e.key.keysym.sym;
			if (key == SDLK_F11 && button_fscreen){ button_fscreen = false; }
			(*m_player).KeyEvent(key, false);
		}
		if (e.type == SDL_MOUSEMOTION){
			(*m_player).MMotionEvt(e.motion.xrel, e.motion.yrel);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN){
			(*m_player).MouseClick(e.button.x, e.button.y, (int)e.button.button);
		}
		if (e.type == SDL_MOUSEBUTTONUP){
			(*m_player).MouseRelease((int)e.button.button);
		}
		if (e.type == SDL_QUIT){
			m_isClosed = true;
			(*this).~Display();//Close faster
		}
	}
}