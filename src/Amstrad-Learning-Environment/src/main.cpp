/* *****************************************************************************
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
* *****************************************************************************
* A.M.L.E (Amstrad Learning Environment)
* Copyright (c) 2020 by Kévin Durou, Johann Kadionik, Vincent Ameeuw, Wilfried 
* Augeard and Theo De Castro Pinto.
* Released under the GNU General Public License; see License.txt for details.
* 
* A.L.E (Arcade Learning Environment)
* Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
* the Reinforcement Learning and Artificial Intelligence Laboratory
* Released under the GNU General Public License; see License.txt for details.
*
* Based on: Caprice32 -- "An Amstrad CPC Emulator"
* (c) Copyright 1997-2015 Ulrich Doewich
* (c) Copyright 2016-2019 Colin Pitrat
*
* *****************************************************************************
*/

#include <cstdlib>
#include <iostream>
#include <thread>
#include "limits.h"
#include <stdio.h>

#include "types.h"
#include "keyboard.h"
#include "InputReader.h"

#include "amle_interface.h"

#include <chrono>
#include <thread>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

/**
//Use to call a function without arguments in a thread
void tmp(InputReader amle_input_reader)
{
    amle_input_reader.displayInputs();  
}
**/

SDL_Event getKeyEvent(SDLKey code, Uint8 type, SDLMod mod = KMOD_NONE) {
    SDL_Event sdlevent;
    sdlevent.type = type;
    sdlevent.key.keysym.sym = code;
    sdlevent.key.keysym.mod = mod;
    return sdlevent;
}

void addEventToBuffer(std::vector<SDL_Event>* buffer, SDLKey code, SDLMod mod = KMOD_NONE) {
    buffer->push_back(getKeyEvent(code, SDL_KEYDOWN, mod));
    buffer->push_back(getKeyEvent(code, SDL_KEYUP, mod));
}

int getElapsedTimeLastInput() {
    std::chrono::time_point<std::chrono::system_clock> currTime = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currTime-inputTimer).count();
}

SDL_Event getRandomEvent(std::vector<SDL_Event> possibleEvents) {
    return possibleEvents[rand() % possibleEvents.size()];
}

std::vector<SDL_Event> getPossibleEvents() {
    std::vector<SDL_Event> possibleEvents;
    addEventToBuffer(&possibleEvents, SDLK_x);
    addEventToBuffer(&possibleEvents, SDLK_z);
    addEventToBuffer(&possibleEvents, SDLK_SPACE);
    addEventToBuffer(&possibleEvents, SDLK_1);
    addEventToBuffer(&possibleEvents, SDLK_RETURN);
    return possibleEvents;
}

void normalRun(int argc, char ** argv) {
    AmLEInterface amle(argc, argv);
    while(true) {
        amle.step();
    }
}

void testRun(int argc, char ** argv) {
    srand(time(0));
    AmLEInterface amle(argc, argv);

    while(getElapsedTimeLastInput() <= 2000)
        amle.step();

    // amle.loadROM(SupportedGames::Buggy, "buggy.dsk");
    amle.loadSnapshot(SupportedGames::Buggy, "buggy.sna");

    std::vector<SDL_Event> possibleEvents = amle.getLegalActions();

    while(getElapsedTimeLastInput() <= 5000)
        amle.step();

    // CPCScreen pixels = getScreen();

    while(true) {
        if(getElapsedTimeLastInput() >= 2 * dwTicksOffset) {
            SDL_Event event = getRandomEvent(possibleEvents);
            amle.act(&event);
            inputTimer = std::chrono::system_clock::now();
        }

        amle.step();

        if(amle.gameOver()) {
            std::cout << amle.getScore() << std::endl;
            break;
        }
    }
}

int main(int argc, char ** argv)
{   
    testRun(argc, argv);
    return EXIT_SUCCESS;
}
