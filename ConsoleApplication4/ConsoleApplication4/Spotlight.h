#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "glm\glm.hpp"

class Spotlight{
public:
	glm::vec3 m_pos, m_color, m_direction;
	double m_intensity, m_range, m_radius;
	Spotlight(){//Set up default variables
		m_pos.x = 0; m_pos.y = 1; m_pos.z = 0;
		m_color.r = 1; m_color.g = 1; m_color.b = 1;
		m_intensity = 1; m_range = 8; m_radius = 0.4;
		m_direction.x = 1; m_direction.y = 0; m_direction.z = 0;
	}

	void SetDirection(double xco, double yco, double zco);
	void SetPosition(double xco, double yco, double zco);
	void MovePosition(double xco, double yco, double zco);
	void SetColor(double r, double g, double b);
};

#endif