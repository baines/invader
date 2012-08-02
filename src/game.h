#ifndef _GAME_H_
#define _GAME_H_
#include "gamestate.h"
#include "sound.h"
#include "particles.h"
#include <cmath>
#include <list>
#include "text.h"

class Bullet;
class Invader;
class Barrier;

class Game : public Gamestate {
	public:
		Game(StateMgr& mgr);
		~Game();
		void update(Input& input, Uint32 delta);
		void draw(Ringbuff<Sprite*>& gfx);
		void invaderbh();
	private:
	    typedef std::list<Bullet*>::iterator bullet_it;
	    typedef std::list<ParticleFx*>::iterator particle_it;
		Invader* inv;
		MultiSprite *player, *hp;
		Sound *shoot, *hit, *ouch, *die;
		std::list<ParticleFx*> particles;
		std::list<Bullet*> bullets;
		Barrier** barriers;
		Text* stext;
		unsigned int timer;
		int score;
		int ended;
};

#endif
