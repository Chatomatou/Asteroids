#pragma once

#include <string>
#include <cstdint>
 

const std::string TITLE{ "Asteroides" };
constexpr uint32_t W{ 800 };
constexpr uint32_t H{ 600 };
constexpr uint32_t ENEMY_NUMBER{ 10 };
constexpr uint32_t BORDER{ 50 };
constexpr uint32_t UNIFORM_SPEED_MIN{ 200 };
constexpr uint32_t UNIFORM_SPEED_MAX{ 300 };

enum class SDL_APP_EXIT_CODE
{
	SDL_APP_EXIT_CODE_SUCCESS = 0,
	SDL_APP_EXIT_CODE_FAILURE = 1
};