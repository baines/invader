#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "SDL/SDL.h"
#include "glcontext.h"
#include "texturemgr.h"

class Sprite;
template<class A> class Ringbuff;

class Renderer {
	public:
		Renderer(int w, int h, const char* caption);
		void draw(Ringbuff<Sprite*>& gfx);
		void toggleFullscreen();
		void toggleBorder();
		inline const int vsyncOn() const { return vsync; }
	protected:
		void reload(int w, int h, int flags);
		TextureMgr textureMgr;
		GLContext gl;
		SDL_Surface* screen;
		int fullscreen_w, fullscreen_h, windowed_w, windowed_h, vsync;
};

#endif
