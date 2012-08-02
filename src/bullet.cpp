#include "bullet.h"
#include <cmath>

Bullet::Bullet(float x, float y, float xv, float yv, int ttl) 
	: MultiSprite(NULL, x, y, 1, 6, 1, 1), xVel(xv), yVel(yv) {
	this->ttl = ttl + (rand() % ttl);
}

Bullet::Bullet(const MultiSprite& m, float xv, float yv, int ttl) :
    MultiSprite(m), xVel(xv), yVel(yv){
    this->ttl = ttl + (rand() % ttl);
}

void Bullet::update(unsigned int delta){
	x += xVel * (delta / 10.0f);
	y += yVel * (delta / 10.0f);
		
	ttl -= delta;
}
