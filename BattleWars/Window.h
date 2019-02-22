#pragma once

#include <SDL.h>
#include <memory>
#include <fstream>

class Window
{
public:
	static Window& GetWindowInstance();
	bool getError() const noexcept;
	bool windowShouldClose() const noexcept;
	int pollEvent(SDL_Event& events) const noexcept;
	void close() noexcept;
	void clear(const uint8_t r = 0, const uint8_t g = 0, const uint8_t b = 0, const uint8_t a = 0);
	void present() const noexcept;

	std::shared_ptr<SDL_Renderer>& getRendererHandle() noexcept;
private:
	Window();
	bool m_initializationOk{ true };
	bool m_close{ false };
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_windowHandle;
	std::shared_ptr<SDL_Renderer> m_rendererHandle;
};
