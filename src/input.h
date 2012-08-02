#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_mouse.h>

#define MOUSE_LEFT SDL_BUTTON(SDL_BUTTON_LEFT)
#define MOUSE_RIGHT SDL_BUTTON(SDL_BUTTON_RIGHT)

struct Mouse {
	int x;
	int y;
	int buttons;
};

class Input {
	public:
		Uint8* keys;
		Uint8 mods;
		struct Mouse mouse;
		char* text;
		int textLen;
		
		Input() : keys(SDL_GetKeyState(0)){
			enteringText = textMax = textLen = 0;
			text = NULL;
		}
		
		void updateMouseButtons(Uint8 button, Uint8 state){
			if(state){
				mouse.buttons |= button;
			} else {
				mouse.buttons &= ~button;
			} 
		}
		
		void updateMousePosition(Uint16 x, Uint16 y){
			mouse.x = x;
			mouse.y = y;
		}
		
		void updateModifiers(Uint8 mods){
			this->mods = mods;
		}
		
		void addText(SDL_keysym& keysym){
			if(!enteringText){
				return;
			} 
			switch(keysym.sym){
				case SDLK_BACKSPACE:
					if(textLen > 0){
						textLen--;
						text[textLen] = '\0';
					}
					break;
				default:
					if(textLen < textMax && keysym.unicode && !(keysym.unicode & 0xFF80)){
						text[textLen] = keysym.unicode & 0x7f;
						textLen++;
					}
					break;
			}
		}
		
		void setTextEntry(Uint8 enable, int maxLength){
			if(enteringText == enable){
				return;
			}
			SDL_EnableUNICODE(enable);
			SDL_EnableKeyRepeat(enable ? 400 : 0, 30);
			enteringText = enable;
			if(enable){
				text = new char[maxLength+1];
				memset(text, '\0', maxLength+1);
				textLen = 0;
				textMax = maxLength;
			} else {
				delete [] text;
				text = NULL;
			}	
		}
	private:
		int enteringText, textMax, oldmods;
};

#endif
