#pragma once

#include "SDL3/SDL_events.h"

struct Input
{
public:
	static bool GetKey(unsigned short key)
	{
		return keyboard[key] & 0b110000;
	}
	static bool GetKeyDown(unsigned short key)
	{
		return keyboard[key] == 0b111100;
	}
	static bool GetKeyUp(unsigned short key)
	{
		return keyboard[key] & 0b000011;
	}

	static SDL_MouseMotionEvent motion;

private:
	friend class Window;
	static unsigned char keyboard[512];
	static unsigned char mouse[3];
};
