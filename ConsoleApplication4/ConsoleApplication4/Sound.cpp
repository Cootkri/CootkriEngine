#include "Sound.h"

void Sound::Init(char* path, float volume, int channel){
	pos[0] = 0; pos[1] = 0; pos[2] = 0;
	range = 25;
	m_channel = channel;
	m_path = path;
	volume = (volume > 1.0f || volume < 0.0f ? 1.0f : volume);
	m_volume = (MIX_MAX_VOLUME * volume);
}

Sound::~Sound(){
	if (m_sound == NULL){ return; }
	Mix_FreeChunk(m_sound);
}

void Sound::Load(){
	m_sound = Mix_LoadWAV(m_path);
	if (m_sound == NULL){
		std::cout << "ERROR: Sound '" << m_path << "' couldn't be loaded!" << std::endl;
		return;
	}
	Mix_VolumeChunk(m_sound, m_volume);
}

void Sound::Unload(){
	if (m_sound == NULL){ return; }
	Mix_FreeChunk(m_sound);
}

void Sound::Play(){
	if (m_sound == NULL){ return; }
	Mix_VolumeChunk(m_sound, m_volume);
	Mix_PlayChannel(m_channel, m_sound, 0);
}

void Sound::Emit(glm::vec3 relpos){
	if (m_sound == NULL){ return; }
	Update(relpos);
	Mix_PlayChannel(m_channel, m_sound, 0);
}

void Sound::Update(glm::vec3 relpos){//Update volume based on distance
	if (m_sound == NULL){ return; }
	double distance = sqrt(pow(relpos.x - pos[0], 2) + pow(relpos.y - pos[1], 2) + pow(relpos.z - pos[2], 2));
	if (distance <= range){
		Mix_VolumeChunk(m_sound, (m_volume * (1 - sqrt(distance) / sqrt(range))));
	}else{
		Mix_VolumeChunk(m_sound, 0);
	}
}

