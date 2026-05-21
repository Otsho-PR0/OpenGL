#pragma once

#include <SDL3/SDL_video.h>

#include "Input.hpp"

class Window
{
public:
	Window(const Window&) = delete;
	Window() = delete;
	Window(const char*, int, int, SDL_WindowFlags = 0u);
	~Window();

	void operator=(const Window&) = delete;

	SDL_Window* GetWindow();
	SDL_GLContext GetContext();

	void Show();
	void Hide();
	void Close();
	void GrabMouse(bool enabled);
	bool IsMouseGrabed();

	void Swap();

	void GetSize(int*, int*);
	void PollEvents(void(*)(SDL_Event*) = nullptr);

	bool& ShouldClose();

private:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;

	bool m_ShouldClose = false;
};
