#include "Ship.h"
#include "Constant.h"


Ship::Ship(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path) : SpaceElement(renderer, path), Animation{renderer, "assets/images/Explosion.png", 60, 4, 4}
{

}

void Ship::draw(std::shared_ptr<SDL_Renderer>& renderer)
{
	SpaceElement::draw(renderer);
	SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);

 
	for (auto& bullet : m_bulletsInformation)
	{
		SDL_Rect rect{ bullet.x, bullet.y, DIMENSION_BULLET, DIMENSION_BULLET };
		SDL_RenderDrawRect(renderer.get(), &rect);
	}

	if (m_dead)
	{
		Animation::draw(renderer, m_x, m_y, m_w, m_h);
		m_bulletsInformation.erase(m_bulletsInformation.begin(), m_bulletsInformation.end());
	}
 
}

void Ship::update(SDL_Event& events, const float dt)
{
	Animation::update(dt * 1000);

	if (!m_dead)
	{
		if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RIGHT)
			m_angle += ANGULAR_SPEED * dt;
		else if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_LEFT)
			m_angle -= ANGULAR_SPEED * dt;
		else if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_UP)
		{
			(m_vx < MAX_SPEED) ? m_vx = m_vx + SPEED : m_vx = MAX_SPEED;
			(m_vy < MAX_SPEED) ? m_vy = m_vy + SPEED : m_vy = MAX_SPEED;
		}
		else if (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_RCTRL)
		{
			Bullet bullet;
			bullet.angle = m_angle + 180.0;
			bullet.x = m_x + std::cos((bullet.angle * M_PI) / 180.0f) * BULLET_SPEED * dt;

			bullet.y = m_y + std::sin((bullet.angle * M_PI) / 180.0f) * BULLET_SPEED * dt;
			m_bulletsInformation.push_back(bullet);
			Mix_PlayChannel(1, m_shoot.get(), 0);
		}

		(m_vx > 0) ? m_vx = m_vx - FRICTION * dt : m_vx = 0;
		(m_vy > 0) ? m_vy = m_vy - FRICTION * dt : m_vy = 0;

		auto angle = m_angle + 180.0;
		m_x = m_x + std::cos((angle * M_PI) / 180.0) * m_vx * dt;
		m_y = m_y + std::sin((angle * M_PI) / 180.0) * m_vy * dt;


		for (auto& bullet : m_bulletsInformation)
		{
			bullet.x = bullet.x + std::cos((bullet.angle * M_PI) / 180.0f) * BULLET_SPEED * dt;
			bullet.y = bullet.y + std::sin((bullet.angle * M_PI) / 180.0f) * BULLET_SPEED * dt;
		}
	}
	else
	{
	}
	 


	if (m_x < 0)
		m_x = W;
	else if (m_x > W)
		m_x = 0;
	else if (m_y < 0)
		m_y = H;
	else if (m_y > H)
		m_y = 0;	 

}

void Ship::dead()
{
	m_dead = true;
}

bool Ship::isDead()
{
	return m_dead;
}

std::vector<Bullet>& Ship::getVectorBullet()
{
	return m_bulletsInformation;
}

void Ship::alive()
{
	m_dead = false;
}
void Ship::stop()
{
	m_x = 0;
	m_y = 0;
}
