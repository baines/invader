#ifndef _TITLE_H_
#define _TITLE_H_
#include "gamestate.h"
#include "sprite.h"
#include "game.h"
#include "text.h"

class Title : public Gamestate {
	public:
		Title(StateMgr& mgr) : Gamestate(mgr){
			title = new MultiSprite("title.png", 160, 120, 128, 128, 1, 2);
			cointext = new MultiSprite("title.png", 160, 200, 128, 32, 1, 8);
			cointext->setFrame(0, 4);
			myname = new Text(275, 8, 8, 8, "2011 Alex Baines");
		}
		void update(Input& input, Uint32 delta){
			if(input.keys[SDLK_SPACE] || input.keys[SDLK_RETURN]){
			    input.keys[SDLK_SPACE] = false;
			    input.keys[SDLK_RETURN] = false;
				statemgr.pop(1);
				statemgr.push(new Game(statemgr));
			}
			timer += delta;
			if(timer > 1000) timer = 0;
		}
		void draw(Ringbuff<Sprite*>& gfx){
			title->draw(gfx);
			myname->draw(gfx);
			if(timer < 500)	cointext->draw(gfx);
		}
	private:
		MultiSprite *title, *cointext;
		Text* myname;
		Uint32 timer;
};

#endif
