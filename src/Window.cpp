#include <Window.hpp>
#include <SDL3/SDL_video.h>

#include <SDL3/SDL_events.h>
#include <glad/gl.h>
#include <SDL3/SDL_opengl.h>

Window::Window(const char* title, int width, int height, SDL_WindowFlags flags)
{
	m_Window = SDL_CreateWindow(title, width, height, flags | SDL_WINDOW_OPENGL);
	m_Context = SDL_GL_CreateContext(m_Window);
	SDL_GL_SetSwapInterval(1);
	gladLoadGL(SDL_GL_GetProcAddress);
}

Window::~Window()
{
	Close();
}

SDL_Window* Window::GetWindow()
{
	return m_Window;
}

SDL_GLContext Window::GetContext()
{
	return m_Context;
}

void Window::Show()
{
	SDL_ShowWindow(m_Window);
}

void Window::Hide()
{
	SDL_HideWindow(m_Window);
}

void Window::Close()
{
	if (m_Window)
	{
		m_ShouldClose = false;
		SDL_DestroyWindow(m_Window);
		SDL_GL_DestroyContext(m_Context);
		m_Window = nullptr;
	}
}

void Window::GrabMouse(bool enabled)
{
	SDL_SetWindowRelativeMouseMode(m_Window, enabled);
}

bool Window::IsMouseGrabed()
{
	return SDL_GetWindowRelativeMouseMode(m_Window);
}

void Window::Swap()
{
	SDL_GL_SwapWindow(m_Window);
}

void Window::GetSize(int* w, int* h)
{
	SDL_GetWindowSize(m_Window, w, h);
}

void Window::PollEvents(void(*callback)(SDL_Event*))
{
	SDL_Event event;
	Input::motion.xrel = 0.0f;
	Input::motion.yrel = 0.0f;
	for (unsigned char& key : Input::keyboard)
		key &= 0b110000;
	while (SDL_PollEvent(&event))
	{
		if (callback)
			callback(&event);
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			m_ShouldClose = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (Input::keyboard[event.key.scancode] ^ 0b110000)
				Input::keyboard[event.key.scancode] = 0b111100;
			break;
		case SDL_EVENT_KEY_UP:
			Input::keyboard[event.key.scancode] = 0b000011;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			if (SDL_GetWindowRelativeMouseMode(m_Window))
				Input::motion = event.motion;
			break;
		}
	}
}

bool& Window::ShouldClose()
{
	return m_ShouldClose;
}
