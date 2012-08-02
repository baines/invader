#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
#include "ringbuff.h"
#include "input.h"
#include "statemgr.h"
#include "sprite.h"
#include <SDL/SDL_stdinc.h>

class Renderer;
class Sound;
class Sprite;

class Gamestate {
	public:
		Gamestate(StateMgr& mgr) : statemgr(mgr) {}
		virtual ~Gamestate(){};
		virtual void update(Input& input, Uint32 delta) 	= 0;
		virtual void draw(Ringbuff<Sprite*>& gfx) 		= 0;
	protected:
		StateMgr& statemgr;
};

#endif
