#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#include "sprite.h"
#include "ringbuff.h"
#include "sound.h"

class ParticleFx {
    public:
        ParticleFx(float x, float y, float v, float dir, float spread, int ttl, int num = 20);
        ~ParticleFx();
        void update(Uint32 delta);
        void draw(Ringbuff<Sprite*>& gfx);
        void setFrame(int fx, int fy);
        bool done;
    private:
        class Particle : public MultiSprite {
            public:
                Particle(float x, float y, float xv, float yv, int ttl) :
                    MultiSprite("sprites.png", x, y, 2, 2, 16, 16), xv(xv), yv(yv), ttl(ttl) {
                        setFrame(3, 2);
                    }
                float xv, yv, spread, ttl;
        };
        Particle** particles;
        int num;
};

#endif
