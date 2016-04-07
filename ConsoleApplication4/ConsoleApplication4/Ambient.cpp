#include "Ambient.h"

void Ambient::SetPosition(double xco, double yco, double zco){ m_position.x = (float)xco; m_position.y = (float)yco; m_position.z = (float)zco; }

void Ambient::MovePosition(double xco, double yco, double zco){//Relative to current
	m_position.x += (float)xco; m_position.y += (float)yco; m_position.z += (float)zco;
}

void Ambient::SetColor(double r, double g, double b){ m_color.x = (float)r; m_color.y = (float)g; m_color.z = (float)b; }