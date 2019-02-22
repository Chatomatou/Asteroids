#pragma once

#include <SDL_mixer.h>
#include <vector>

#include "Bullet.h"
#include "SpaceElement.h"
#include "Animation.h"


class Ship : public SpaceElement, public Animation
{
public:
	Ship(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path);
	virtual void draw(std::shared_ptr<SDL_Renderer>& renderer) override;
	virtual void update(SDL_Event& events, const float dt) override;

	void dead();
	bool isDead();
	void alive();
	void stop();

	std::vector<Bullet>& getVectorBullet();
	static constexpr uint32_t DIMENSION_BULLET{ 10 };

private:
	bool m_engineOn{ false };
	bool m_dead{ false };
	uint32_t m_timer{ 0 };
	std::vector<Bullet> m_bulletsInformation;
	
	std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> m_shoot{ Mix_LoadWAV("assets/audios/sfx_wpn_laser8.wav"), &Mix_FreeChunk };
 
	static constexpr float BULLET_SPEED{ 500 };
	 
	static constexpr float SPEED{ 10.f };
	static constexpr uint32_t MAX_SPEED{ 300 };
	static constexpr float ANGULAR_SPEED{ 250.f };
	static constexpr float FRICTION{ 60 };
};