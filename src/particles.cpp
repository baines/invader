#include "particles.h"
#include <cmath>

ParticleFx::ParticleFx(float x, float y, float v, float dir, float sp, int ttl, int num)
        : done(false), num(num){
    particles = new Particle*[num];
    for(int i = 0; i < num; ++i){
        float d = dir-(sp/2.0f) + (sp * ((rand()%1000)/1000.0f));
        float t = (ttl*0.8f) + ((rand()%ttl)-(ttl/2));
        float v2 = (v*0.4f) + ((v*0.6f) * ((rand()%1000)/1000.0f));
        particles[i] = new Particle(x, y, v2*sin(d * (M_PI/180.0f)), v2*cos(d * (M_PI/180.0f)), t);
    }
}

void ParticleFx::setFrame(int fx, int fy){
     for(int i = 0; i < num; ++i){
        particles[i]->setFrame(fx, fy);
    }
}

ParticleFx::~ParticleFx(){
     for(int i = 0; i < num; ++i){
        delete particles[i];
     }
     delete [] particles;
}

void ParticleFx::update(Uint32 delta){
    int totalttl = 0;
    
    for(int i = 0; i < num; ++i){
        if(particles[i]->y > 240){
            particles[i]->yv *= -0.35f;
            particles[i]->y = 240;
        }
        particles[i]->yv += 0.2 * (delta/20.0f);
        particles[i]->x += particles[i]->xv * (delta/50.0f);
        particles[i]->y += particles[i]->yv * (delta/50.0f);
        if(particles[i]->ttl > 0){
            particles[i]->ttl -= delta;
        }
        totalttl += particles[i]->ttl;
    }

    if(totalttl <= 0) done = true;
}

void ParticleFx::draw(Ringbuff<Sprite*>& gfx){
     for(int i = 0; i < num; ++i){
        if(particles[i]->ttl > 0) particles[i]->draw(gfx);
     }
}

