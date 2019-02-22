#include "SpaceElement.h"
#include "Constant.h"

SpaceElement::SpaceElement(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path) : m_texture{nullptr, &SDL_DestroyTexture}
{
	std::ofstream fLogErr{ "SpaceElementErr.txt" };
 	auto s = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(IMG_Load(path.data()), &SDL_FreeSurface);

	if (s == nullptr)
	{
		fLogErr << SDL_GetError();
	}
 
	m_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTextureFromSurface(renderer.get(), s.get()), &SDL_DestroyTexture);

	if (m_texture == nullptr)
	{
		fLogErr << SDL_GetError();
	}

	SDL_QueryTexture(m_texture.get(), nullptr, nullptr, &m_w, &m_h);
 
}

void SpaceElement::draw(std::shared_ptr<SDL_Renderer>& renderer)  
{
	SDL_Rect position{ static_cast<int>(m_x), static_cast<int>(m_y), m_w, m_h };
	SDL_Point center{ m_w / 2, m_h / 2 };
	SDL_RendererFlip flip{ static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL) };

	SDL_RenderCopyEx(renderer.get(), m_texture.get(), nullptr, &position, m_angle, &center, flip);
}

void SpaceElement::update(SDL_Event& events, const float dt)
{
	m_x = m_x + std::cos(m_angle * M_PI / 180.0) * m_vx * dt;
	m_y = m_y + std::sin(m_angle * M_PI / 180.0) * m_vx * dt;

	if (m_x < 0)
		m_x = W;
	else if (m_x > W)
		m_x = 0;
	else if (m_y < 0)
		m_y = H;
	else if (m_y > H)
		m_y = 0;

}

void SpaceElement::setX(const float x)
{
	m_x = x;
}
void SpaceElement::setY(const float y)
{
	m_y = y;
}
void SpaceElement::setAngle(const float angle)
{
	m_angle = angle;
}
void SpaceElement::setVelocityX(const float vx)
{
	m_vx = vx;
}
void SpaceElement::setVelocityY(const float vy)
{
	m_vy = vy;
}
 
float SpaceElement::getX() const
{
	return m_x;
}
float SpaceElement::getY() const
{
	return m_y;
}
int SpaceElement::getW() const
{
	return m_w;
}
int SpaceElement::getH() const
{
	return m_h;
}
void SpaceElement::setW(const int w)
{
	m_w = w;
}
void SpaceElement::setH(const int h)
{
	m_h = h;
}