#ifndef _BARRIER_H_
#define _BARRIER_H_

#include "sprite.h"
#include "ringbuff.h"

class Barrier {
    public:
        Barrier(float x, float y);
        void update(Uint32 delta);
        void draw(Ringbuff<Sprite*>& gfx);
        bool checkCollision(Sprite* s);
    private:
        class Piece : public MultiSprite {
            public:
            Piece(float x, float y) 
            : MultiSprite("sprites.png", x, y, 4, 4, 16, 16){
                visible = true;
                setFrame(3, 3);
            }
            bool visible;
        };
        Piece** pieces;
};

#endif
