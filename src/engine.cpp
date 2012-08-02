#include <ctime>
#include "engine.h"
#include "title.h"
#include "util.h"

Engine::Engine() : renderer(640, 480, "Invader from Space"), gfx(1024), 
        running(true), delta(0){
	memset(&e, 0, sizeof(e));
	statemgr.push(new Title(statemgr));
	time = SDL_GetTicks();
}

bool Engine::run(){	
	while (SDL_PollEvent(&e)){
		switch (e.type){
			case SDL_QUIT: 
				running = false; 
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN:
				input.updateModifiers(e.key.keysym.mod);
				handleKeyPress(e.key.state);
				break;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				input.updateMouseButtons(e.button.button, e.button.state);
				break;
			case SDL_MOUSEMOTION:
				input.updateMousePosition(e.motion.x, e.motion.y);
				break;				
			default:
				break;
		}
	}
	
	while((delta = SDL_GetTicks() - time) < (renderer.vsyncOn() ? 1 : 16)){
		SDL_Delay(1);
	}
	time += delta;
	
	statemgr.update(input, delta);
	statemgr.draw(gfx);
	renderer.draw(gfx);
	
	return running;
}

void Engine::handleKeyPress(Uint8 toggle){
	switch(e.key.keysym.sym) {
		case SDLK_ESCAPE:
			running = false;
			break;
		case SDLK_RETURN: 
			if(toggle && (e.key.keysym.mod & KMOD_ALT)) {
			    SDL_ShowCursor(SDL_ShowCursor(-1) ^ 1);
				renderer.toggleFullscreen();
			}
			break;
		/*case SDLK_g:
			if(toggle && (e.key.keysym.mod & KMOD_CTRL)){
				SDL_WM_GrabInput((SDL_GrabMode)(SDL_WM_GrabInput(SDL_GRAB_QUERY) ^ 1));
				SDL_ShowCursor(SDL_ShowCursor(-1) ^ 1);
			}
			break;*/
		default:
			break;
	}
	if(toggle) input.addText(e.key.keysym);
}

Engine::~Engine(){
	SDL_Quit();
}

int main(int argc, char **argv){	
	srand(time(0));
	Engine engine;
	while(engine.run());
	
	return 0;
}
