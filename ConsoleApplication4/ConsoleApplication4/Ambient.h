#ifndef AMBIENT_H
#define AMBIENT_H

#include "glm\glm.hpp"

class Ambient{
public:
	glm::vec3 m_position, m_color;
	double m_intensity, m_range;

	Ambient(){//Set up default variables
		m_position.x = 0; m_position.y = 1; m_position.z = 0;
		m_color.r = 1; m_color.g = 1; m_color.b = 1;
		m_intensity = 1; m_range = 10;
	}

	void SetPosition(double xco, double yco, double zco);
	void MovePosition(double xco, double yco, double zco);
	void SetColor(double r, double g, double b);
};

#endif