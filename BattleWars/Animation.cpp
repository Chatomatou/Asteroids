#include "Animation.h"

Animation::Animation(std::shared_ptr<SDL_Renderer>& renderer, const std::string& path, const unsigned int delay, const unsigned int spriteX, const unsigned int spriteY)
{
	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> m_spritesheet{ IMG_Load(path.data()), &SDL_FreeSurface };

	if (m_spritesheet == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Animation : %s\n", IMG_GetError());
		return;
	}

	auto offsetX = m_spritesheet->w / spriteX;
	auto offsetY = m_spritesheet->h / spriteY;
	m_delay = delay;

	for (auto i = 0; i < m_spritesheet->h; i += offsetY)
	{
		for (auto j = 0; j < m_spritesheet->w; j += offsetX)
		{
			SDL_Rect src{ j, i, offsetX, offsetY };
			std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> sprite{SDL_CreateRGBSurface(0, offsetX, offsetY, 32, 0, 0, 0, 0), &SDL_FreeSurface};
			
			if (sprite == nullptr)
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Animation : %s\n", SDL_GetError());

			SDL_BlitSurface(m_spritesheet.get(), &src, sprite.get(), nullptr);

			SDL_SetColorKey(sprite.get(), SDL_TRUE, SDL_MapRGB(sprite->format, 0, 0, 0));

			std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture{ SDL_CreateTextureFromSurface(renderer.get(), sprite.get()), &SDL_DestroyTexture };


			if (m_texture == nullptr)
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Animation : %s\n", SDL_GetError());
			
			m_keyframes.push_back(std::move(m_texture));

		}
	}

}


void Animation::update(const float dt)
{
	m_timer += dt;

	if (m_timer > m_delay)
	{
		m_timer = 0;
		
		if (m_index == m_keyframes.size() - 1)
			m_index = 0;
		else
			m_index++;
	}
}
void Animation::draw(std::shared_ptr<SDL_Renderer>& renderer, const int x, const int y, const int w, const int h)
{
	SDL_Rect dst{ x, y, w, h };
	SDL_RenderCopy(renderer.get(), m_keyframes[m_index].get(), nullptr, &dst);
}