#include "Window.h"
#include "Constant.h"

Window::Window() : m_windowHandle{ nullptr, &SDL_DestroyWindow }, m_rendererHandle{ nullptr, &SDL_DestroyRenderer }
{
	std::ofstream fLogErr{ "WindowErr.txt" };

	m_windowHandle = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(TITLE.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN), &SDL_DestroyWindow);

	if (m_windowHandle == nullptr)
	{
		fLogErr << SDL_GetError();
		m_initializationOk = false;
	}

	m_rendererHandle = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(m_windowHandle.get(), -1, SDL_RENDERER_ACCELERATED), &SDL_DestroyRenderer);

	if (m_rendererHandle == nullptr)
	{
		fLogErr << SDL_GetError();
		m_initializationOk = false;
	}
}

Window& Window::GetWindowInstance()
{
	static Window hWnd;
	return hWnd;
}

bool Window::getError() const noexcept
{
	return !m_initializationOk;
}

bool Window::windowShouldClose() const noexcept
{
	return !m_close;
}

int Window::pollEvent(SDL_Event& events) const noexcept
{
	return SDL_PollEvent(&events);
}
void Window::close() noexcept
{
	m_close = true;
}
void Window::clear(const uint8_t r , const uint8_t g , const uint8_t b, const uint8_t a)
{
	SDL_SetRenderDrawColor(m_rendererHandle.get(), r, g, b, a);
	SDL_RenderClear(m_rendererHandle.get());
}
void Window::present() const noexcept
{
	SDL_RenderPresent(m_rendererHandle.get());
}

std::shared_ptr<SDL_Renderer>& Window::getRendererHandle() noexcept
{
	return m_rendererHandle;
}
