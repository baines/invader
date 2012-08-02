#ifndef _TEXT_H_
#define _TEXT_H_
#include "sprite.h"
#include "ringbuff.h"
#include <cstring>

class Text {
	public:
		Text(float x, float y, float w, float h, int i){
			char str[32];
			sprintf(str, "%d", i);
			init(str, x, y, w, h);
		}
		Text(float x, float y, float w, float h, const char* format, ...)
		{
			char str[256];
			va_list args;
			va_start (args, format);
			vsnprintf (str, 256, format, args);
			va_end (args);
			init(str, x, y, w, h);
		}

		void draw(Ringbuff<Sprite*>& gfx){
			for(int x = 0; x < wordLength; x++){
				gfx.push(letters[x]);
			}
		}
		~Text(){
			for(int x = 0; x < wordLength; x++){
				delete letters[x];
			}
			delete [] letters;
		}
	private:
		void init(const char* word, float x, float y, float w, float h){
			wordLength = strlen(word);

			letters = new Sprite*[wordLength];
			
			float letterX = x - ((w/2) * wordLength * 0.8f);
			
			for(int x = 0; x < wordLength; x++){
				letters[x] = new Sprite("font.png", letterX, y, w, h);
				
				int fontX = ((int)word[x] - 32) % 16;
				int fontY = ((int)word[x] - 32) / 16;
								
				letters[x]->setTextureBounds((float)fontX / 16, (float)fontY / 8
					, (float)(fontX+1) / 16, (float)(fontY+1) / 8);
				letterX += w * 0.8f;
				letterX = (int)letterX;
			}
		}
		Sprite** letters;
		int wordLength;
};

#endif
