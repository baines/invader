#include <cstdio>
#include "renderer.h"
#include "ringbuff.h"
#include "sprite.h"

Renderer::Renderer(int w, int h,const char* caption):textureMgr(gl),gl(320,240){
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0){
		fprintf(stderr, "Video error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_GL_LoadLibrary(NULL);
	SDL_WM_SetCaption(caption, NULL);
	
	fullscreen_w = SDL_GetVideoInfo()->current_w;
	fullscreen_h = SDL_GetVideoInfo()->current_h;
	windowed_w = w;
	windowed_h = h;
		
	reload(w, h, SDL_OPENGL | SDL_ANYFORMAT);
	SDL_Delay(200);
}

void Renderer::reload(int w, int h, int flags){
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	
	if (!(screen = SDL_SetVideoMode(w, h, 32, flags))){
		fprintf(stderr, "Video error: %s\n", SDL_GetError());
		exit(1);
	}
	gl.createContext(w, h);
	textureMgr.reload();
	
	vsync = 0;
	SDL_GL_GetAttribute(SDL_GL_SWAP_CONTROL, &vsync);
}

void Renderer::toggleFullscreen() {
	int flags = screen->flags ^ SDL_FULLSCREEN;
	
	if((flags & SDL_FULLSCREEN) == SDL_FULLSCREEN){
		reload(fullscreen_w, fullscreen_h, flags);
	} else {
		reload(windowed_w, windowed_h, flags);
	}
}

inline void Renderer::toggleBorder(){
	reload(screen->w, screen->h, screen->flags ^ SDL_NOFRAME);
}

void Renderer::draw(Ringbuff<Sprite*>& gfx){
	Sprite* s = gfx.front();
	int tex = 0;
	float w,h;
	
	gl.Clear(GL_COLOR_BUFFER_BIT);
	
	while(!gfx.empty()){    
        if(s->txr.id == -1) s->txr.id = textureMgr.load(s->txr.path);
	    tex = s->txr.id;
	    gl.BindTexture(GL_TEXTURE_2D, tex);
	    
	    gl.Begin(GL_QUADS);
	    
	    while(!gfx.empty() && s->txr.id == tex) {
	        	    
		    w = s->w / 2.0f;
		    h = s->h / 2.0f;
				
		    gl.TexCoord2f(s->txr.x1, s->txr.y1); gl.Vertex2f(s->x-w,s->y-h);
		    gl.TexCoord2f(s->txr.x2, s->txr.y1); gl.Vertex2f(s->x+w,s->y-h);
		    gl.TexCoord2f(s->txr.x2, s->txr.y2); gl.Vertex2f(s->x+w,s->y+h);
		    gl.TexCoord2f(s->txr.x1, s->txr.y2); gl.Vertex2f(s->x-w,s->y+h);
		
		    gfx.pop();
		    s = gfx.front();
	    }
	    
	    gl.End();
	}
    //Border
    gl.BindTexture(GL_TEXTURE_2D, 0);
	gl.Begin(GL_LINE_LOOP); 
	gl.Vertex2f(0,0); gl.Vertex2f(320,0); 
	gl.Vertex2f(320,240); gl.Vertex2f(0, 240);
	gl.End();
	
	SDL_GL_SwapBuffers();
}

