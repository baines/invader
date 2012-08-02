#include "texturemgr.h"
#include "SDL/SDL_image.h"
#include <string>

unsigned int TextureMgr::load(const char* path) {
	GLuint texture;
	
	std::map<const char*, GLuint, util::strLess>::const_iterator texIter 
		= textures.find(path);

	if(texIter != textures.end()){
		texture = texIter->second;
	} else {		
		texture = loadFromFile(path, 0);
		textures.insert(std::pair<const char*,GLuint>(path,texture));
	}
	
	return texture;
}

unsigned int TextureMgr::loadFromFile(const char* filename, unsigned int id) {
	std::string path("./data/img/");
	path += filename;
	
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface){
		fprintf(stderr, "%s missing or corrupt.\n", path.c_str());
		exit(1);
	}

	GLuint texture;
	
	if(id == 0){
		gl.GenTextures(1, &texture);
	} else {
		texture = id;
	}
	
	gl.BindTexture(GL_TEXTURE_2D, texture);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	gl.TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
		
	SDL_FreeSurface(surface);	
	return texture;
}

void TextureMgr::reload(){
	for(std::map<const char*, GLuint, util::strLess>::const_iterator itr 
		= textures.begin(); itr != textures.end(); ++itr){
		loadFromFile(itr->first, itr->second);
	}
}

void TextureMgr::unload(const char* path) {
	std::map<const char*, GLuint, util::strLess>::iterator texItr
		= textures.find(path);
	
	if(texItr != textures.end()){
		gl.DeleteTextures(1, &texItr->second);
		textures.erase(texItr);
	}
}
