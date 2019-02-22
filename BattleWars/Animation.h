#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>
#include <vector>

class Animation
{
public:
	Animation(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path, const unsigned int delay, const unsigned int spriteX, const unsigned int spriteY);
protected:
	unsigned int m_delay;
	unsigned int m_timer;
	unsigned int m_index;

	void update(const float dt);
	void draw(std::shared_ptr<SDL_Renderer>& renderer, const int x, const int y, const int w, const int h);
	std::vector<std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> m_keyframes;
};