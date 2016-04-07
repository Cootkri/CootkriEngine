#ifndef SPRITE_H
#define SPRITE_H

#include "Image.h"

class Sprite{
public:
	Sprite(double w, double h, const char* texture, Shader* shader){
		m_yield = true;
		width = w; height = h;
		img.Init(0, 0, w, h, texture, shader);
		m_posi[0] = 0; m_posi[1] = 0; m_posi[2] = 0;
	}
	void Teleport(double xco, double yco, double zco){ img.Teleport(xco, yco, zco); m_posi[0] = xco; m_posi[1] = yco;  m_posi[2] = zco; }
	void SetYield(bool set){ m_yield = set; }
	void SetCam(Camera *cam1){ cam = cam1; }
	void Draw(Player &plyr){
		double xco = plyr.GetX() - m_posi[0], zco = plyr.GetZ() - m_posi[2];
		double result = atan(abs(xco) / abs(zco)), hyp = 0;
		if (zco < 0 && xco > 0){ result = PI - result; }
		if (zco < 0 && xco < 0){ result += PI; }
		if (zco > 0 && xco < 0){ result = 2 * PI - result; }
		if (m_yield){
			hyp = sqrt(pow(xco, 2) + pow(plyr.GetZ(), 2));
			hyp = atan((plyr.GetY() - m_posi[1]) / hyp);
		}
		img.Rotate(-hyp, result, 0);
		img.Teleport(m_posi[0] + sin(result)*width/2, m_posi[1], m_posi[2] + cos(result)*width/2);
		img.DrawCam(*cam);
	}

protected:
private:
	Image img;
	Camera* cam;
	double width, height, m_posi[3];
	bool m_yield;
};

#endif