#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include "glm\glm.hpp"
#include "SDL2\SDL.h"
#include "SDL2\SDL_mixer.h"

class Sound{
public:
	glm::vec3 pos; double range;
	Sound(){}
	~Sound();
	void Init(char* path, float volume, int channel=-1);
	void Load();
	void Unload();
	void Play();
	void Emit(glm::vec3 relpos); void Update(glm::vec3 relpos);
protected:
private:
	bool m_playing, m_paused;
	char* m_path;
	int m_channel, m_volume;
	Mix_Chunk *m_sound = NULL;
};

#endif