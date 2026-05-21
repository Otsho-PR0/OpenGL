#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <Game.hpp>

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	Game game;
	App::AddLayer((AppLayer*)&game);
	return App::Run();
}
