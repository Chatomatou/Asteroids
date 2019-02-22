#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>
#include <fstream>
#include <cmath>
 

class SpaceElement
{
public:
	SpaceElement(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path);
	virtual void draw(std::shared_ptr<SDL_Renderer>& renderer);
	virtual void update(SDL_Event& events, const float dt);

	float getX() const;
	float getY() const;
	int getW() const;
	int getH() const;

	void setX(const float x);
	void setY(const float y);
	void setAngle(const float angle);
	void setVelocityX(const float vx);
	void setVelocityY(const float vy);
	void setW(const int w);
	void setH(const int h);

 
protected:
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
	float m_x;
	float m_y;
	int m_w;
	int m_h;
	float m_vx;
	float m_vy;
	float m_angle;
};