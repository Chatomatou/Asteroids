#include <SDL.h>
#include <SDL_mixer.h>
#include <Windows.h>
#include <vector>
#include <random>
#include <ctime>

#include "Constant.h"
#include "Window.h" 
#include "SpaceElement.h"
#include "Ship.h"
 

using Event = SDL_Event;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return static_cast<int>(SDL_APP_EXIT_CODE::SDL_APP_EXIT_CODE_FAILURE);

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
	{
		SDL_Quit();
		return static_cast<int>(SDL_APP_EXIT_CODE::SDL_APP_EXIT_CODE_FAILURE);
	}

	if (Window::GetWindowInstance().getError())
	{
		Mix_CloseAudio();
		SDL_Quit();
		return static_cast<int>(SDL_APP_EXIT_CODE::SDL_APP_EXIT_CODE_FAILURE);
	}
 
	Event events;
	int dt{ 0 };

	std::vector<std::string> pathAnimation{"assets/images/1.png", "assets/images/2.png", "assets/images/3.png"};

 
	Ship ship{ Window::GetWindowInstance().getRendererHandle(), "assets/images/ship.png" };
	std::vector<SpaceElement*> elementsSpace;

	std::default_random_engine engine{static_cast<unsigned int>(std::time(nullptr))};
	std::uniform_int_distribution<uint32_t> uniformX(BORDER, W - BORDER);
	std::uniform_int_distribution<uint32_t> uniformY(BORDER, H - BORDER);
	std::uniform_int_distribution<uint32_t> uniformAngle(0, 360);
	std::uniform_int_distribution<uint32_t> uniformSpeed{ UNIFORM_SPEED_MIN, UNIFORM_SPEED_MAX };

	for (auto i{ 0 }; i < ENEMY_NUMBER; i++)
	{
		SpaceElement* object = new SpaceElement{ Window::GetWindowInstance().getRendererHandle(), "assets/images/asteroid.png" };
		
		object->setX(uniformX(engine));
		object->setY(uniformY(engine));
		object->setAngle(uniformAngle(engine));
		object->setVelocityX(uniformSpeed(engine));
		object->setVelocityY(uniformSpeed(engine));

		elementsSpace.push_back(object);
	}


	// Music
	Mix_AllocateChannels(3);
	std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> m_deadSound{ Mix_LoadWAV("assets/audios/sfx_exp_short_hard5.wav"), &Mix_FreeChunk };
	std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> music{ Mix_LoadMUS("assets/musics/Yeah.mp3"), Mix_FreeMusic };
 
	Mix_PlayMusic(music.get(), -1);

	SDL_Rect collision{};

	while (Window::GetWindowInstance().windowShouldClose())
	{
		auto lastTicks = SDL_GetTicks();
 

		while (Window::GetWindowInstance().pollEvent(events)) // events
		{
			if (events.window.event == SDL_WINDOWEVENT_CLOSE)
				Window::GetWindowInstance().close();
			if (events.type == SDL_KEYDOWN && ship.isDead())
				ship.setX(0), ship.setY(0), ship.alive(), ship.stop();
		}

	 
		// Update 
		for (auto* asteroid : elementsSpace)
		{
			SDL_Rect rectElement{ asteroid->getX(), asteroid->getY(), asteroid->getW(), asteroid->getH() };
			SDL_Rect rectShip{ ship.getX(), ship.getY(), ship.getW(), ship.getH() };
			asteroid->update(events, dt / 1000.0f);

			if (SDL_HasIntersection(&rectElement, &rectShip))
			{
				ship.dead();
				collision.x = ship.getX();
				collision.y = ship.getY();
				collision.w = ship.getW();
				collision.h = ship.getH();
			}
		}

 
		for (auto& bullet : ship.getVectorBullet())
		{
			SDL_Rect rectBullet{ bullet.x, bullet.y, Ship::DIMENSION_BULLET, Ship::DIMENSION_BULLET };
			auto i = 0;
			for (auto* asteroid : elementsSpace)
			{
				
				SDL_Rect rectAsteroid{ asteroid->getX(), asteroid->getY(), asteroid->getW(), asteroid->getH() };
				if (SDL_HasIntersection(&rectBullet, &rectAsteroid))
				{
					asteroid->setW(asteroid->getW() - 0.1);
					asteroid->setH(asteroid->getH() - 0.1);
					Mix_PlayChannel(2, m_deadSound.get(), 0);

					if (asteroid->getW() < 20 && asteroid->getH() < 20) 
						elementsSpace.erase(elementsSpace.begin() + i);
					
				}
				i++;
			}

			 
		}
		 

		ship.update(events, dt / 1000.0f);
		 
		// Draw 
		Window::GetWindowInstance().clear();
		for (auto* asteroid : elementsSpace)
		{
			asteroid->draw(Window::GetWindowInstance().getRendererHandle());


		}

		ship.draw(Window::GetWindowInstance().getRendererHandle());
		Window::GetWindowInstance().present();

	 
		if ((SDL_GetTicks() - lastTicks) < (1000 / 60))
			SDL_Delay((1000 / 60) - (SDL_GetTicks() - lastTicks));
		 
		dt = SDL_GetTicks() - lastTicks;
 	}


	for (auto* asteroid : elementsSpace)
		delete asteroid;

	Mix_CloseAudio();
	SDL_Quit();
	return static_cast<int>(SDL_APP_EXIT_CODE::SDL_APP_EXIT_CODE_SUCCESS);
}