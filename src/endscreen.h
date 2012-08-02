#ifndef _ENDSCREEN_H_
#define _ENDSCREEN_H_
#include "gamestate.h"
#include "sprite.h"
#include "title.h"

class Endscreen : public Gamestate {
	public:
		Endscreen(StateMgr& mgr, bool win, int score) : Gamestate(mgr){
			title = new MultiSprite("title.png", 160, 40, 128, (win) ? 32 : 64, 1, (win) ? 8 : 4);
			title->setFrame(0, (win) ? 5 : 3);
			timer = 0;
			stext = new Text(170, 120, 16, 16, "Score: %d", score);
		}
		~Endscreen(){
		    delete title;
		    delete stext;
		}
		void update(Input& input, Uint32 delta){
			if(timer > 10000 || input.keys[SDLK_RETURN]){
			    input.keys[SDLK_RETURN] = false;
				statemgr.pop(1);
				statemgr.push(new Title(statemgr));
			}
			timer += delta;
		}
		void draw(Ringbuff<Sprite*>& gfx){
			title->draw(gfx);
			stext->draw(gfx);
		}
	private:
		MultiSprite *title;
		Text* stext;
		unsigned int timer;
};

#endif
