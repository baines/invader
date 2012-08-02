#ifndef _BULLET_H_
#define _BULLET_H_
#include "sprite.h"

class Bullet : public MultiSprite {
	public:
		Bullet(float x, float y, float xv, float yv, int ttl);
		Bullet(const MultiSprite& m, float xv, float yv, int ttl);
		~Bullet(){};
		void update(unsigned int delta);
		int ttl;
	private:
		float xVel;
		float yVel;
};

#endif		
