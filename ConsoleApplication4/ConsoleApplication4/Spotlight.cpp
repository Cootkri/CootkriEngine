#include "Spotlight.h"

void Spotlight::SetDirection(double xco, double yco, double zco){
	m_direction.x = (float)xco; m_direction.y = (float)yco; m_direction.z = (float)zco;
}

void Spotlight::SetPosition(double xco, double yco, double zco){ m_pos.x = (float)xco; m_pos.y = (float)yco; m_pos.z = (float)zco; }

void Spotlight::MovePosition(double xco, double yco, double zco){//Relative to current
	m_pos.x += (float)xco; m_pos.y += (float)yco; m_pos.z += (float)zco;
}

void Spotlight::SetColor(double r, double g, double b){ m_color.x = (float)r; m_color.y = (float)g; m_color.z = (float)b; }