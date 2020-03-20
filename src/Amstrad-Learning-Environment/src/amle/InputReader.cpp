#include "InputReader.h"

InputReader::InputReader()
{
    this->lctrl = false;
    this->lshift = false;
    this->p = false;
    
}

void InputReader::setFlags(SDL_Event event)
{
    switch (event.type) 
    {
        case SDL_KEYDOWN:
        {
            if(event.key.keysym.sym == SDLK_LCTRL)
            {
                this->lctrl = true;
            }

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {
                this->lshift = true;
            }

            if(event.key.keysym.sym == SDLK_p)
            {
                this->p = true;
            }
        }
        break;

        case SDL_KEYUP: 
        {
            if(event.key.keysym.sym == SDLK_LCTRL)
            {
                this->lctrl = false;
            }

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {
                this->lshift = false;
            }

            if(event.key.keysym.sym == SDLK_p)
            {
                this->p = false;
            }
        }
        break;
    }
}

void InputReader::readFlags(MemoryReader * memoryReader)
{
    // If those two keys are not pressed, the flags are not set for the AmLE
    if(this->lctrl && this->lshift)
    {
        if(this->p) 
        {
            memoryReader->printScore();
            printf("\n");
        }
    }
}

void InputReader::handleEvents(SDL_Event event, MemoryReader * memoryReader)
{
    setFlags(event);
    readFlags(memoryReader);
}

void InputReader::displayInputs()
{
    SDL_Event inEvent;
    
    while(getReading())
    { 
        
        int ret = SDL_PeepEvents(&inEvent, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN));
        if(ret > 0)
        {
            for(int i = 0; i < ret; i++)
            {
                std::cout << SDL_GetKeyName(inEvent.key.keysym.sym) << std::endl;
            }
        }
    }
}

void InputReader::setReading(bool read){
    this->reading = read;
}

bool InputReader::getReading(){
    return this->reading;
}