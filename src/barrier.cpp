#include "barrier.h"

Barrier::Barrier(float x, float y){
    pieces = new Piece*[38];
    int xs[38] = { 2, 4, 6, 8, 10, 12,
                   0, 2, 4, 6, 8, 10, 12, 14,
                   0, 2, 4, 6, 8, 10, 12, 14,
                   0, 2, 4, 6, 8, 10, 12, 14,
                   0, 2, 12, 14, 0, 2, 12, 14 };
    int ys[38] = { 1, 1, 0, 0, 1, 1,
                   3, 3, 3, 2, 2, 3, 3, 3,
                   5, 5, 5, 4, 4, 5, 5, 5,
                   7, 7, 7, 6, 6, 7, 7, 7,
                   9, 9, 9, 9, 11, 11, 11, 11 };
    
    for(int i = 0; i < 38; ++i){
        pieces[i] = new Piece(x + (2*xs[i]), y + (2*ys[i]));
    }
}

void Barrier::update(Uint32 delta){


}

bool Barrier::checkCollision(Sprite* s){
    for(int i = 37; i >= 0; --i){
        if(pieces[i]->visible && Sprite::collision(s, pieces[i])){
            pieces[i]->visible = false;
            return true;
        }
    }
    return false;
}

void Barrier::draw(Ringbuff<Sprite*>& gfx){
    for(int i = 0; i < 38; ++i){
        if(pieces[i]->visible) pieces[i]->draw(gfx);
    }
}
