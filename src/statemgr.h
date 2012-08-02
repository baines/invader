#ifndef _STATEMGR_H_
#define _STATEMGR_H_
#include <vector>
#include <queue>
#include "ringbuff.h"
#include <SDL/SDL_stdinc.h>
class Sprite;
class Gamestate;
class Input;

class StateMgr {
	public:
		StateMgr();
		~StateMgr();
		void update(Input& input, Uint32 delta);
		void draw(Ringbuff<Sprite*>& gfx);
		void pop(int amount);
		void push(Gamestate* state);
	private:	
		std::vector<Gamestate*> states;
		std::queue<Gamestate*> nextStates;
		int popAmount;
		
};

#endif
	
