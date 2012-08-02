#ifndef _SOUND_H_
#define _SOUND_H_
#include <SDL/SDL_mixer.h>

class Sound {
    public:
        virtual void play() ;
        virtual void stop() const;
        static Sound* load(const char* name);
        ~Sound();
    protected:
        Sound(Mix_Chunk* chunk);
        Mix_Chunk* chunk;
        int chan;
};

class SoundStub : public Sound {
    public:
        SoundStub(Mix_Chunk* chunk);
        void play();
        void stop() const;
};

#endif

