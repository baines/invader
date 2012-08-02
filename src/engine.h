#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "SDL/SDL.h"
#include "ringbuff.h"
#include "statemgr.h"
#include "renderer.h"
#include "input.h"

class Sound;
class Sprite;
	
class Engine {
	public:
		Engine();
		~Engine();
		bool run();
	private:
		void handleKeyPress(Uint8 toggle);
		
		Renderer renderer;
		StateMgr statemgr;
		Input input;
		//Resource resource;
		Ringbuff<Sprite*> gfx;
		
		bool running;
		SDL_Event e;
		int time, delta;
};

#endif
