#ifndef _GL_CONTEXT_H_
#define _GL_CONTEXT_H_
#define _GL_STRING(name) #name
#define _GL(name) _GL_STRING(gl##name)
#define GL_LOADFUNC(gl, name) gl.loadFunction(_GL(name), (void**)&gl.name)
#include "opengl.h"
#include "util.h"

class GLContext {
	public:
		GLContext(int w, int h) : initial_w(w), initial_h(h){
			#define GL_FUNC(type, name, args) \
				name = 0;
			#include "opengl.h"
			#undef GL_FUNC
		}
		void createContext(int w, int h){
			loadAllFuncs();			
			MatrixMode(GL_PROJECTION);
	        int scale = util::min(h / initial_h, w / initial_w);
	        //float scale = (float)h / (float)initial_h; //non pixel-perfect scale
	        int xOff = (w - (initial_w * scale)) / 2;
	        int yOff = (h - (initial_h * scale)) / 2;
	        Viewport(xOff, yOff, initial_w * scale, initial_h * scale);
			LoadIdentity();
			Ortho(0, initial_w, initial_h, 0, -1, 1);
			MatrixMode(GL_MODELVIEW);
			Enable(GL_TEXTURE_2D);
			Enable(GL_BLEND);
			BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			LineWidth(4.0f);
		}
		bool loadFunction(const char* name, void** ptr){
			*ptr = SDL_GL_GetProcAddress(name);
			if(*ptr){
				return true;
			} else {
				printf("OpenGL function %s not available.\n", name);
				return false;
			}
		}
				
		bool loadAllFuncs(){
			bool retval = true;
			#define GL_FUNC(type, name, args) \
				retval = retval && loadFunction(_GL(name), (void**)&name);
			#include "opengl.h"
			#undef GL_FUNC
			return retval;
		}

		#define GL_FUNC(type, name, args) \
			name##_p name;
		#include "opengl.h"
		#undef GL_FUNC
	private:
	    int initial_w, initial_h;
};

#endif
