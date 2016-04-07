#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "transform.h"
#include "camera.h"
#include "Ambient.h"
#include "Spotlight.h"

#define MAX_AMBIENTS	64
#define MAX_SPOTLIGHTS	64

extern Ambient* ambs[MAX_AMBIENTS]; extern Spotlight* spots[MAX_SPOTLIGHTS];
extern unsigned int ambCount, spotCount;

class Shader{
public:
	//Ambient* ambs[MAX_AMBIENTS]; unsigned int ambCount;

	void Init(const std::string& fileName);
	Shader(){ ambCount = 0; for (unsigned int i = 0; i < MAX_AMBIENTS; i++){ ambs[i] = 0; }
		spotCount = 0; for (unsigned int i = 0; i < MAX_SPOTLIGHTS; i++){ spots[i] = 0; }
		m_program = NULL;
		effect = 0;
	}

	void Bind();
	void SetEffect(int set);
	void Update(const Transform& transform);//FOR 2D DRAWING
	void Update(const Transform& transform, const Camera& camera);
	virtual ~Shader();
	void UpdateAmbients();//Spotlights as well
	void AddAmbient(Ambient* amb), RemoveAmbient(Ambient* amb);
	void AddSpotlight(Spotlight* spot), RemoveSpotlight(Spotlight* spot);

	GLuint effect;
protected:
private:
	glm::vec4 ambients[MAX_AMBIENTS],  ambientc[MAX_AMBIENTS];
	glm::vec4 splights[MAX_SPOTLIGHTS], splightc[MAX_SPOTLIGHTS], splightdir[MAX_SPOTLIGHTS];
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other){}
	void operator=(const Shader& other){}

	enum{
		TRANSFORM_U,
		POSIFORM_U,
		AMBIENT_U,
		AMBIENTC_U,
		SPLIGHT_U,
		SPLIGHTC_U,
		SPLIGHTDIR_U,
		EFFECT_U,
		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};

#endif