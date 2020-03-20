#ifndef INPUT_READER_H_
#define INPUT_READER_H_

#include <iostream>
#include "SDL.h"
#include "DataFetcher.h"


class InputReader
{
    public:

    /**
     * Default constructor.
     **/
    InputReader();

    /**
     * \fn setFlags(SDL_Event event)
     * \brief Changes the flags of each key if required. i.e: If left control is pressed, lctrl is set to true, if
     * P is released, p is set to false etc...
     * \param event: The event to handle.
     **/
    void setFlags(SDL_Event event);

    /**
     * \fn readFlags(MemoryReader* memoryReader)
     * \brief Reads teh current flags and calls the required methods in consequence.
     * \param memoryReader: An object tha has already gathered all the important information prior to this call.
     **/
    void readFlags(MemoryReader * memoryReader);

    /**
     * void handleEvents(SDL_Event event, MemoryReader * memoryReader)
     * \brief Handles the events sent through Caprice32.
     * \param event: The event to handle.
     * \param memoryReader: An object tha has already gathered all the important information prior to this call.
     **/
    void handleEvents(SDL_Event event, MemoryReader * memoryReader);

    /**
     * \fn void displayInputs()
     * \brief Display inputs entered by user without touching the setup queue by SDL. 
     * \param void
     **/
    void displayInputs();

    /**
     * \fn void setReading(bool read)
     * \brief Setter of reading private variable. 
     * \param read: True means we want to read (and display) keyboard inputs. False we want to stop displaying inputs.
     **/
    void setReading(bool read);

     /**
     * \fn bool getReading()
     * \brief Getter of reading private variable. 
     * \return the value of reading.
     **/
    bool getReading();

    private:

        /** 
        * \var lctrl: If not pressed, all the other inputs are ignored.
        * \var lshift: If not pressed, all the other inputs are ignored.
        * \var p: If pressed, will display important information of the game (lifes, score, round etc).
        * \var reading: True means we want to read (and display) keyboard inputs. 
        **/
        bool lctrl;
        bool lshift;
        bool p;
        bool reading;
};


#endif