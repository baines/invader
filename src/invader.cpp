#include "invader.h"
#include "sound.h"
#include "particles.h"
#include <cmath>
#include <cstring>
#include <algorithm>

Invader::Invader() : MultiSprite("sprites.png", 32, 32, 16, 8, 2, 8), state(STANDARD), 
        hp(149), timer(0), shieldTimer(0), dir(1), stateCounter(0), flipSprite(true){
        stateOrder = new int[9];
        int order[9] = { STANDARD, FAST, GROW, FAST, MINIONS, FAST, LAZER, FAST, MINIONS2 };
        memcpy(stateOrder, order, 9*sizeof(int));
        move1 = Sound::load("move1.wav");
        move2 = Sound::load("move2.wav");
        bang  = Sound::load("bang.wav");
    	hit   = Sound::load("hit1.wav");
    	hit2  = Sound::load("hit2.wav");
    	grow  = Sound::load("grow.wav");
    	shoot2 = Sound::load("shoot2.wav");
    	lazer = Sound::load("l1.wav");
    	lazer2 = Sound::load("l2.wav");
    	shield = new MultiSprite("sprites.png", 32, 32, 16, 16, 2, 4);
    	shield->setFrame(0, 3);
}

Invader::~Invader(){
    delete move1;
    delete move2;
    delete bang;
    delete hit;
    delete hit2;
    delete grow;
    delete shoot2;
    delete lazer;
    delete lazer2;
    delete shield;
    delete [] stateOrder;
    std::for_each(minions.begin(), minions.end(), util::deleter());
    std::for_each(bullets.begin(), bullets.end(), util::deleter());
	std::for_each(misc.begin(), misc.end(), util::deleter());
}

bool Invader::checkCollision(Sprite* s, int* ctype){
    if(Sprite::collision(this, s)){
        if(shieldTimer <= 0){
            --hp;
            hit->play();
            *ctype = (state > 0) ? 1 : 0;
            if((hp % 10) == 0){
                bang->play();
                state = stateOrder[++stateCounter % 9];
                shieldTimer = 1500;
                timer = 0;
            }
        } else {
            *ctype = 2;
            hit2->play();
        }
        return true;
    }
    for(minion_it i = minions.begin(); i != minions.end(); ++i){
        if(Sprite::collision((*i), s)){
            (*i)->hp--;
            hit->play();
            if((*i)->hp <= 0){
                delete (*i);
                i = minions.erase(i);
                --i;
            }
            *ctype = 1;
            return true;
        }
    }
    return false;
}

bool Invader::hitTest(Sprite* s, int* damage){
    for(bullet_it i = bullets.begin(); i != bullets.end(); ++i){
        if(Sprite::collision((*i), s)){
            *damage = 50;
			delete (*i);			
			i = bullets.erase(i);
			--i;
            return true;
        }
    }
    for(sprite_it i = misc.begin(); i != misc.end(); ++i){
        if((int)(*i)->h == 8 && Sprite::collision((*i), s)){
            *damage = 200;
			delete (*i);			
			i = misc.erase(i);
			--i;
            return true;
        }
    }
    return false;
}

bool Invader::barrierCollide(Barrier* b, float* x, float* y){
    bool res = false;
    for(bullet_it i = bullets.begin(); i != bullets.end(); ++i){
        if(b->checkCollision((*i))){
			hit->play();
			*x = (*i)->x;
			*y = (*i)->y;
			delete (*i);			
			i = bullets.erase(i);
			--i;
			res = true;
		}
    }
    for(sprite_it i = misc.begin(); i != misc.end(); ++i){
        if((int)(*i)->h == 8 && b->checkCollision((*i))){
			hit->play();
			*x = (*i)->x;
			*y = (*i)->y;
			delete (*i);			
			i = misc.erase(i);
			--i;
			res = true;
        }
    }
	return res;
}

void Invader::update(Uint32 delta) {
	timer += delta;
	if(shieldTimer > 0) shieldTimer -= delta;
	shield->x = this->x;
	shield->y = this->y;
	for(bullet_it i = bullets.begin(); i != bullets.end(); ++i){
		(*i)->update(delta);
		if((*i)->ttl < 0){
	        delete (*i);
		    i = bullets.erase(i); 
		    --i;
		}
	}
    switch(state){
        case STANDARD   : updateStandard(delta, false); break;
        case FAST       : updateStandard(delta, true);  break;
        case GROW       : updateGrow(delta);            break;
        case MINIONS    : updateMinions(delta, false);  break;
        case MINIONS2   : updateMinions(delta, true);   break;
        case LAZER      : updateLazer(delta);           break;
	}	
}

void Invader::updateStandard(Uint32 delta, bool fast){
    if(timer > (fast ? 150 : 750)){
		if((rand() % 3) == 0){
			bullets.push_back(new Bullet(x, y, 0, 2, 1500));
		    shoot2->play();
		}
        timer = 0;
        setFrame(flipSprite ? 1 : 0, fast ? 3 : 2);
        flipSprite ? move1->play() : move2->play();
        flipSprite = !flipSprite;
        x += dir * (8 + ((int)w >> 3));
        if(x > 320 - (8 + ((int)w >> 3))){
	        dir = -1;
	        y += ((int)h>>2);
        } else if(x < (8 + ((int)w >> 3))){
	        dir = 1;
	        y += ((int)h>>2);
        }
    }
}

void Invader::updateGrow(Uint32 delta){
    static int growth = 0;
    if(w == 128){
        ++state;
        return;
    }
    if(growth == 0 && timer == delta){
        grow->play();
        y+=(int)h>>2;
    }
    if(timer > 100){
        shield->w *= (growth % 2) ? 0.5f : 2;
        shield->h *= (growth % 2) ? 0.5f : 2;
        w *= (growth % 2) ? 0.5f : 2;
        h *= (growth % 2) ? 0.5f : 2;
        timer = 0;
        ++growth;
    }
    if(growth == 5){
        state = stateOrder[++stateCounter % 9];
        growth = 0;
    }
}

void Invader::updateMinions(Uint32 delta, bool tough){
    static int phase = 0;
    static float movex, movey;
    shieldTimer = 100;
    if(timer == delta){
        phase = 1;
        movex = (160 - x)/16.0f;
        movey = (100 - y)/16.0f;
    } else if(phase < 17){
        x += movex;
        y += movey;
        ++phase;
    } else if(phase == 17){
        x = 160;
        y = 100;
        for(int i = 0; i < 6; ++i){
            float r = (i * M_PI) / 3.0f;
            minions.push_back(new Minion(160 + 80*(sin(r)), 100 + 80*(cos(r)), i, tough ? 3 : 1));
        }
        ++phase;
    } else if(phase >= 20 && phase < 37){
        x -= movex;
        y -= movey;
        ++phase;
    } else if(phase == 37){
        shieldTimer = 0;
        state = stateOrder[++stateCounter % 9];
        timer = 0;
    } else if(minions.empty()){
        bang->play();
        phase = 20;
    } else {
        int j = 0;
        for(minion_it i = minions.begin(); i != minions.end(); ++i){
            float r = ((*i)->id * M_PI) / 3.0f;
            (*i)->x = 160 + 80 * (sin(r + (timer/1000.0f)));
            (*i)->y = 100 + 80 * (cos(r + (timer/1000.0f)));
            ++j;
            if(rand()%100 == 0){
                bullets.push_back(new Bullet((*i)->x, (*i)->y, 0, 2, 1500));
                shoot2->play();
            }
        }
    }
}

void Invader::updateLazer(Uint32 delta){
	static int phase = 0;
	if(timer == delta){
	    phase = 0;
	} else if(phase == 0){
		++phase;
		for(int i = 0; i < 4; ++i){
			misc.push_back(new MultiSprite("sprites.png", 46 + 72*i, -160, 32, 240, 2, 16));
			misc.back()->setFrame(1, 0);
		}
		lazer->play();
		int x = rand()%4;
		for(int j = 0; j < 64; ++j){
			bullets.push_back(new Bullet(30+ 2*(j%16) + 72*x, -100-20*(j/16), 0, 4, 1000));
		}
	} else if(phase < 80){
		for(sprite_it i = misc.begin(); i != misc.end(); ++i){
			(*i)->y += 8;
		}
		++phase;
	} else if(phase == 80){
		for(sprite_it i = misc.begin(); i != misc.end(); ++i){			
			delete (*i);
			i = misc.erase(i);
			--i;
		}
		for(int j = 0; j < 4; ++j){
		    misc.push_back(new MultiSprite("sprites.png", 46+ j*72, 0, 32, 8, 2, 16));
    		misc.back()->setFrame(1, 1);
        }
		for(int j = 0; j < 5; ++j){
		    misc.push_back(new MultiSprite("sprites.png", 10 + (j*72), 0, 38, 8, 2, 16));
    		misc.back()->setFrame(1, 1);
        }
	    lazer2->play();
		++phase;
	} else if(phase < 125){
		for(sprite_it i = misc.begin(); i != misc.end(); ++i){
			(*i)->y += 6;
		}
		++phase;
	} else {
	    lazer2->stop();
		for(sprite_it i = misc.begin(); i != misc.end(); ++i){			
			delete (*i);
			i = misc.erase(i);
			--i;
		}
        shieldTimer = 0;
        state = stateOrder[++stateCounter % 9];
        timer = 0;
	}
}
        
void Invader::draw(Ringbuff<Sprite*>& gfx){
    util::forEach(minions.begin(), minions.end(), &Minion::draw, gfx);
    util::forEach(bullets.begin(), bullets.end(), &Bullet::draw, gfx);
	util::forEach(misc.begin(), misc.end(), &Sprite::draw, gfx);
    gfx.push(this);
    if(shieldTimer > 0) shield->draw(gfx);
}
