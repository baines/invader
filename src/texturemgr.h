#ifndef _TEXTUREMGR_H_
#define _TEXTUREMGR_H_
#include "SDL/SDL.h"
#include "opengl.h"
#include "util.h"
#include <map>
#include <vector>
#include "glcontext.h"

class TextureMgr {
	public:
		TextureMgr(GLContext& context) : gl(context){}
		void reload();
		unsigned int load(const char* path);
		void unload(const char* path);
	private:
		unsigned int loadFromFile(const char* filename, unsigned int id = 0);
		std::map<const char*, GLuint, util::strLess> textures;
		GLContext& gl;
};

#endif
