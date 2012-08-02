#ifndef _INVADER_H_
#define _INVADER_H_
#include "sprite.h"
#include "bullet.h"
#include "barrier.h"
#include <list>
class Sound;
class ParticleFx;

class Invader : public MultiSprite {
	public:
		Invader();
		~Invader();
		void update(Uint32 delta);
		void draw(Ringbuff<Sprite*>& gfx);
        bool checkCollision(Sprite* other, int* ctype);
		bool hitTest(Sprite* other, int* damage);
		bool barrierCollide(Barrier* b, float* x, float* y);
		int state, hp;
	private:
        void updateStandard(Uint32 delta, bool fast);
        void updateFast(Uint32 delta);
        void updateGrow(Uint32 delta);
        void updateMinions(Uint32 delta, bool tough);
        void updateLazer(Uint32 delta);
        
		class Minion : public MultiSprite {
	    public:
		    Minion(float x, float y, int id, int hp) : MultiSprite("sprites.png", x, y, (hp == 1) ? 8 : 16, 8, (hp == 1) ? 4 : 2, 8){
		        setFrame(1, 1);
		        this->id = id;
		        this->hp = hp;
		    }
		    int id;
		    int hp;
		};
        std::list<MultiSprite*> misc;
		typedef std::list<MultiSprite*>::iterator sprite_it;
		std::list<Bullet*> bullets;
		typedef std::list<Bullet*>::iterator bullet_it;
		std::list<Minion*> minions;
		typedef std::list<Minion*>::iterator minion_it;
		enum {STANDARD = 0, FAST, GROW, MINIONS, MINIONS2, LAZER };
		int timer, shieldTimer, shootTimer, dir, stateCounter, *stateOrder;
		Sound *move1, *move2, *bang, *hit, *hit2, *grow, *shoot2, *lazer, *lazer2;
		bool flipSprite;
		MultiSprite* shield;
		
};		

#endif
