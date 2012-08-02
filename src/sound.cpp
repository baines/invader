#include "sound.h"
#include <SDL/SDL.h>
#include <string>

Sound* Sound::load(const char* name){
    static int loaded = -1;
    
    if(loaded == -1){
        int err = 0;
        err |= SDL_InitSubSystem(SDL_INIT_AUDIO);
        err |= Mix_OpenAudio(44100, AUDIO_S16, 2, 2048);
        if(err){
            loaded = 0;
            fprintf(stderr, "Audio init failed: %s.\n", SDL_GetError());
        } else {
            loaded = 1;
        }
    }
    
    if(loaded){
    	std::string path("./data/snd/");
    	path += name;
	    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
	    if(chunk){
	        return new Sound(chunk);
	    } else {
	        fprintf(stderr, "%s missing or corrupt.\n", path.c_str());
	    }
	}
	return new SoundStub(NULL);
}

inline SoundStub::SoundStub(Mix_Chunk* chunk) : Sound(chunk){}
inline void SoundStub::play() {}
inline void SoundStub::stop() const {}
inline Sound::Sound(Mix_Chunk* chunk) : chunk(chunk){}

Sound::~Sound(){
    if(chunk) Mix_FreeChunk(chunk);
}

inline void Sound::play() {
    chan = Mix_PlayChannel(-1, chunk, 0);
}

inline void Sound::stop() const {
    Mix_HaltChannel(chan);
}
