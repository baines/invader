#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "util.h"
#include "ringbuff.h"
#include <SDL/SDL_stdinc.h>

typedef struct {
	float x1;
	float x2;
	float y1;
	float y2;
	int id;
	const char* path;
} Texture;

class Sprite {
	public:
		float x, y, w, h, rot, alpha;
		Texture txr;
		Sprite(const char* path, float x, float y, float w, float h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;

			txr.x1 = txr.y1 = rot = 0.0f;
			txr.x2 = txr.y2 = alpha = 1.0f;
			txr.id = path == NULL ? 0 : -1;
			txr.path = path;
		}
		virtual void draw(Ringbuff<Sprite*>& gfx){
			gfx.push(this);
		}
		virtual void setTextureBounds(float x1, float y1, float x2, float y2){
			txr.x1 = x1;
			txr.x2 = x2;
			txr.y1 = y1;
			txr.y2 = y2;
		}
		static bool collision(Sprite* one, Sprite* two){
			if((one->x + one->w / 2.0f < two->x - two->w / 2.0f) ||
			   (one->x - one->w / 2.0f > two->x + two->w / 2.0f) ||
			   (one->y + one->h / 2.0f < two->y - two->w / 2.0f) ||
			   (one->y - one->h / 2.0f > two->y + two->w / 2.0f)) {
				return false;
			} else {
				return true;
			}
		}
};

class MultiSprite : public Sprite {
	public:
		MultiSprite(const char* path, float x, float y, float w, float h, int xSegs, int ySegs)
			: Sprite(path, x, y, w, h) {
			
			xSegments = util::max(xSegs, 1);
			ySegments = util::max(ySegs, 1);
			
			setTextureBounds(0, 0, 1.0f/float(xSegments), 1.0f/float(ySegments));
		}
		
		void setFrame(int x, int y){
			float _x = util::max(util::min(x, xSegments - 1), 0);
			float _y = util::max(util::min(y, ySegments - 1), 0);
			
			setTextureBounds(_x/float(xSegments), _y/float(ySegments), (_x+1.0f)/float(xSegments), (_y+1.0f)/float(ySegments));
		}
	private:
		int xSegments;
		int ySegments;
};			

#endif
