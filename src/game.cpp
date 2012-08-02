#include "game.h"
#include "invader.h"
#include "bullet.h"
#include "barrier.h"
#include "endscreen.h"
#include <algorithm>

#define SCR_W 320
#define SCR_H 240

Game::Game(StateMgr& mgr) : Gamestate(mgr), timer(0), ended(0), score(0) {
	player = new MultiSprite("sprites.png", 160, SCR_H - 4, 16, 8, 2, 8);
	hp = new MultiSprite("sprites.png", 160, 8, 300, 4, 16, 16);
	hp->setFrame(2, 2);
	inv = new Invader();
	inv->setFrame(0, 2);
	barriers = new Barrier*[4];
	for(int i = 0; i < 4; ++i) barriers[i] = new Barrier(32 + (72*i), 200);
	shoot = Sound::load("shoot.wav");
	hit   = Sound::load("hit1.wav");
	ouch = Sound::load("ouch.wav");
	die = Sound::load("die.wav");
	stext = new Text(50, 20, 8, 8, "Score: %d", score);
}

Game::~Game(){
    delete player;
    delete inv;
    for(int i = 0; i < 4; ++i) delete barriers[i];
    delete [] barriers;
    delete shoot;
    delete hit;
    delete ouch;
	std::for_each(bullets.begin(), bullets.end(), util::deleter());
	std::for_each(particles.begin(), particles.end(), util::deleter());
}

void Game::update(Input& input, Uint32 delta){
	timer += delta;
	
	if(ended){
	    if(timer > 200){
	        hit->play();
	        timer = 0;
	        particles.push_back(new ParticleFx(inv->x, inv->y, 7, 90+rand()%180, 50, 3000, 10));
	        particles.back()->setFrame(2, 2);
	    }
	    inv->y = (ended == 1) ? inv->y -util::max(1.0f, delta/20.0f) : inv->y +util::max(1.0f, delta/20.0f);
        if(ended == 1 && inv->y + (inv->h/2.0f) < 0){
            statemgr.pop(1);
            statemgr.push(new Endscreen(statemgr, true, score));
        } else if(ended == 2 && inv->y - (inv->h/2.0f) > 240){
            statemgr.pop(1);
            statemgr.push(new Endscreen(statemgr, false, score));
        }
    }
    
	inv->update(delta);
	
	for(bullet_it i = bullets.begin(); i != bullets.end(); ++i){
		(*i)->update(delta);
		int ctype = 0;
		if(inv->checkCollision((*i), &ctype)){
		    score += (ctype == 2) ? -1 : 10;
	        particles.push_back(new ParticleFx(inv->x, inv->y, 7, 90+rand()%180, 50, 3000, 10));
	        if(ctype == 0) particles.back()->setFrame(3, 2);
	        if(ctype == 1) particles.back()->setFrame(2, 2);
	        if(ctype == 2) particles.back()->setFrame(2, 3);
		    delete (*i);
		    i = bullets.erase(i); 
		    --i;
	    } else if((*i)->ttl < 0){
	        delete (*i);
		    i = bullets.erase(i); 
		    --i;
		} else {
			for(int j = 0; j < 4; ++j){
		        if(barriers[j]->checkCollision((*i))){
		            score += 5;
		            particles.push_back(new ParticleFx((*i)->x, (*i)->y, 5, 0, 360, 1000, 10));
		            particles.back()->setFrame(3, 3);
		            hit->play();
		            delete (*i);
		            i = bullets.erase(i); 
		            --i;
		            break;
		        }
		    }
		}
	}
	
	for(particle_it i = particles.begin(); i != particles.end(); ++i){
	    (*i)->update(delta);
	    if((*i)->done){
	        delete (*i);
	        i = particles.erase(i);
	        --i;
	    }
	}
	
	if(ended) return;
	
	if(input.keys[SDLK_SPACE]){
	    score--;
		input.keys[SDLK_SPACE] = false;
		bullets.push_back(new Bullet(player->x, SCR_H - 6, 0, -4, 600));
		shoot->play();
	}
	if(input.keys[SDLK_RIGHT]){
		player->x += input.keys[SDLK_LSHIFT] ? (delta / 11.0f) : (delta / 5.0f);
	} else if(input.keys[SDLK_LEFT]){
		player->x -= input.keys[SDLK_LSHIFT] ? (delta / 11.0f) : (delta / 5.0f);
	}
	
	int damage;
	if(inv->hitTest(player, &damage)){
	    score -= damage;
	    ouch->play();
        particles.push_back(new ParticleFx(player->x, player->y, (damage == 60) ? 10 : 5, 0, 360, 1000, (damage == 60) ? 20 : 10));
        if(damage == 60) particles.back()->setFrame(2, 2);
	    else particles.back()->setFrame(3, 3);
	}
	
	if((int)(hp->w) != (inv->hp*2)){
	    hp->w = inv->hp*2;
	    hp->x = 160 - ((300.0f - hp->w) / 2.0f);
	    particles.push_back(new ParticleFx(hp->x+(hp->w/2.0f), hp->y, 4, 10, 50, 3000, 5));
	    particles.back()->setFrame(2,2);
	}

	for(int j = 0; j < 4; ++j){
		float bx, by;
        if(inv->barrierCollide(barriers[j], &bx, &by)){
            particles.push_back(new ParticleFx(bx, by, 5, 0, 360, 1000, 10));
            particles.back()->setFrame(3, 3);
		}
    }
	
	if(inv->hp <= 1 && !ended){
	    ended = 1;
	    die->play();
        timer = 0;
	}
	
	if(inv->y > 220){
	    ended = 2;
	    die->play();
        timer = 0;
    }
    if(timer > 100){
        delete stext;
        stext = new Text(50, 20, 8, 8, "Score: %d", score);
        timer = 0;
    }

}

void Game::draw(Ringbuff<Sprite*>& gfx){
    for(int i = 0; i < 4; ++i) barriers[i]->draw(gfx);
    stext->draw(gfx);
    hp->draw(gfx);
	inv->draw(gfx);
	player->draw(gfx);
	util::forEach(bullets.begin(), bullets.end(), &Bullet::draw, gfx);
	util::forEach(particles.begin(), particles.end(), &ParticleFx::draw, gfx);
}
