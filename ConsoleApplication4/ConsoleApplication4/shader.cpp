#include "shader.h"

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);
Ambient* ambs[MAX_AMBIENTS]; unsigned int ambCount;
Spotlight* spots[MAX_SPOTLIGHTS]; unsigned int spotCount;

void Shader::Init(const std::string& fileName){
	if (m_program != NULL){//remove previous shaders
		for (unsigned int i = 0; i < NUM_SHADERS; i++){
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}

		glDeleteProgram(m_program);
	}
	else{
		for (unsigned int i = 0; i < MAX_AMBIENTS; i++){
			ambients[i][0] = 0; ambients[i][1] = 0; ambients[i][2] = 0; ambients[i][3] = 0;
			ambientc[i][0] = 1; ambientc[i][1] = 1; ambientc[i][2] = 1; ambientc[i][3] = 6;
		}
		for (unsigned int i = 0; i < MAX_SPOTLIGHTS; i++){
			splights[i][0] = 0; splights[i][1] = 0; splights[i][2] = 0; splights[i][3] = 0;
			splightc[i][0] = 1; splightc[i][1] = 1; splightc[i][2] = 1; splightc[i][3] = 6;
			splightdir[i][0] = 0; splightdir[i][1] = 0; splightdir[i][2] = 1; splightdir[i][3] = 0;
		}
	}
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++){
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
	m_uniforms[POSIFORM_U] = glGetUniformLocation(m_program, "posiform");
	m_uniforms[AMBIENT_U] = glGetUniformLocation(m_program, "ambients");//Positions & Intensity
	m_uniforms[AMBIENTC_U] = glGetUniformLocation(m_program, "ambientc");//Colors & Range
	m_uniforms[EFFECT_U] = glGetUniformLocation(m_program, "effect");
	m_uniforms[SPLIGHT_U] = glGetUniformLocation(m_program, "splights");//Positions & Intensity
	m_uniforms[SPLIGHTC_U] = glGetUniformLocation(m_program, "splightc");//Colors & Range
	m_uniforms[SPLIGHTDIR_U] = glGetUniformLocation(m_program, "splightdir");//Direction Vector & Radius
}

Shader::~Shader(){
	for (unsigned int i = 0; i < NUM_SHADERS; i++){
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::Bind(){
	glUseProgram(m_program);
}

void Shader::SetEffect(int set){
	glUniform1i(m_uniforms[EFFECT_U], set);
	effect = set;
}

void Shader::Update(const Transform& transform, const Camera& camera){
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glm::mat4 posi = transform.GetModel();
	UpdateAmbients();

	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_uniforms[POSIFORM_U], 1, GL_FALSE, &posi[0][0]);
	glUniform4fv(m_uniforms[AMBIENT_U], ambCount, &ambients[0][0]);
	glUniform4fv(m_uniforms[AMBIENTC_U], ambCount, &ambientc[0][0]);
	glUniform4fv(m_uniforms[SPLIGHT_U], spotCount, &splights[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTC_U], spotCount, &splightc[0][0]);
	glUniform4fv(m_uniforms[SPLIGHTDIR_U], spotCount, &splightdir[0][0]);
}

void Shader::Update(const Transform& transform){
	glm::mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

void Shader::AddAmbient(Ambient* amb){
	ambs[ambCount] = amb;
	ambCount++;
}
void Shader::RemoveAmbient(Ambient* amb){
	bool found = false;
	for (unsigned int i = 0; i < ambCount; i++){
		if (ambs[i] == amb){
			found = true;
			ambs[i] = 0;
			continue;
		}
		if (found){
			if (ambs[i] != 0){
				ambs[i - 1] = ambs[i];
				ambs[i] = 0;
			}
			else{
				break;
			}
		}
	}
	if (found){ ambCount--; }
}

void Shader::AddSpotlight(Spotlight* spo){
	spots[spotCount] = spo;
	spotCount++;
}
void Shader::RemoveSpotlight(Spotlight* spo){
	bool found = false;
	for (unsigned int i = 0; i < spotCount; i++){
		if (spots[i] == spo){
			found = true;
			spots[i] = 0;
			continue;
		}
		if (found){
			if (spots[i] != 0){
				spots[i - 1] = spots[i];
				spots[i] = 0;
			}
			else{
				break;
			}
		}
	}
	if (found){ spotCount--; }
}

void Shader::UpdateAmbients(){
	for (unsigned int i = 0; i < ambCount; i++){
		ambients[i][0] = ambs[i]->m_position.x;
		ambients[i][1] = ambs[i]->m_position.y;
		ambients[i][2] = ambs[i]->m_position.z;
		ambients[i][3] = ambs[i]->m_intensity;
		ambientc[i][0] = ambs[i]->m_color.r;
		ambientc[i][1] = ambs[i]->m_color.g;
		ambientc[i][2] = ambs[i]->m_color.b;
		ambientc[i][3] = ambs[i]->m_range;
	}
	for (unsigned int i = 0; i < spotCount; i++){
		splights[i][0] = spots[i]->m_pos.x; splights[i][1] = spots[i]->m_pos.y; splights[i][2] = spots[i]->m_pos.z; splights[i][3] = spots[i]->m_intensity;
		splightc[i][0] = spots[i]->m_color.r; splightc[i][1] = spots[i]->m_color.g; splightc[i][2] = spots[i]->m_color.b; splightc[i][3] = spots[i]->m_range;
		splightdir[i][0] = spots[i]->m_direction.x; splightdir[i][1] = spots[i]->m_direction.y; splightdir[i][2] = spots[i]->m_direction.z; splightdir[i][3] = spots[i]->m_radius;
	}
}

static GLuint CreateShader(const std::string& text, GLenum shaderType){
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0){
		std::cerr << "ERROR: Shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed: ");

	return shader;
}

static std::string LoadShader(const std::string& fileName){
	std::ifstream file;
	file.open((fileName).c_str());
	std::string output, line;
	if (file.is_open()){
		while (file.good()){
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}
	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage){
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram){
		glGetProgramiv(shader, flag, &success);
	}
	else{
		glGetShaderiv(shader, flag, &success);
	}
	if (success == GL_FALSE){
		if (isProgram){
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cerr << errorMessage << ":  '" << error << "'" << std::endl;
	}
}