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
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Caprice32  --  "An Amstrad CPC Emulator"
 * (c) Copyright 1997-2015 Ulrich Doewich
 * (c) Copyright 2016-2019 Colin Pitrat
 *
 * *****************************************************************************
 *  amle_interface.h
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef __AMLE_INTERFACE_H__
#define __AMLE_INTERFACE_H__

/**
 * \file amle_interface.h 
 * \brief This file contains a class that interfaces Caprice32. This file is heavily inspired by the 
 * ale_interface.cpp. More info : https://github.com/mgbellemare/Arcade-Learning-Environment
 * Moreover, it also uses already written code from the Caprice32 source code, more info : 
 * https://github.com/ColinPitrat/caprice32
 */ 

#define NO_PATH "" /**< the value of the path to the current game when no game is loaded. */

#include "cap32Dup.h"
#include "cap32Variables.h"
#include "SupportedGamesUtils.h"
#include "MemoryUtils.h"

/**
 * \brief This class is an interface that has been created mainly so it can be used to 
 * create a new environment within OpenAI Gym. It mainly allows the user to send inputs to 
 * the emulator (Caprice32) and allows the user to get important information about the 
 * game.
 */
class AmLEInterface {
    public:

        /**
         * \fn AmLEInterface()
         * \brief Default constructor. Almost all of the code of this method is shamelessly copied from 
         * \param path: The path to the current directory the python file is.
         * the cap32.cpp file from the Caprice32 emulator source code.
         */ 
        AmLEInterface(const char * path=NO_PATH);

        /**
         * \fn ~AmLEInterface()
         * \brief Default destructor.
         */ 
        ~AmLEInterface();

        /**
         * \fn void loadROM(SupportedGames game, const char * path)
         * \brief This methods loads a game rom a DSK file. This is not guaranteed to work and 
         * this interface is mainly made to work with SNA files instead.
         * \param game: The game to load.
         * \param path: The path to the game to load.
         * \deprecated This method is deprecated because of performance issues and it being inaccurate. 
         * Use loadSnapshot instead.
         */ 
        void loadROM(SupportedGames game, const char * path);

        /**
         * \fn int act(SDL_Event *event)
         * \brief Sends an input to execute before the next step.
         * \param event: The input to send.
         * \return The reward the AI should consider after its action (not accurate yet).
         */ 
        int act(SDL_Event *event);

        /**
         * \fn void resetGame()
         * \brief Resets the current game. If no game has been loaded yet, restarts the emulator.
         */ 
        void resetGame();

        /**
         * \fn void loadSnapshot(SupportedGames game, const char * path)
         * \brief Loads a SNA file into memory. This is the best way to start a game.
         * \param game: The game to load.
         * \param path: The path to the SNA file.
         */ 
        void loadSnapshot(SupportedGames game, const char * path);

        /**
         * \fn void saveSnapshot(const char * path)
         * \brief Saves a snapshot of the current state of the memory. 
         * \param path: The path to where the file should be saved.
         */ 
        void saveSnapshot(const char * path);

        /**
         * \fn int getNbLives()
         * \brief Returns the number of lives currently, if any.
         * \return The number of lives currently, if any.
         */ 
        int getNbLives();

        /**
         * \fn bool gameOver()
         * \brief Determines of the game has ended.
         * \return True if the game has ended, false otherwise.
         */ 
        bool gameOver();

        /**
         * \fn int getScore()
         * \brief Returns the current score, if any.
         * \return The current score, if any.
         */ 
        int getScore();

        /**
         * \fn std::vector<SDL_Event> getLegalActions()
         * \brief Returns the list of legal actions in the current game.
         * \return The list of legal actions in the current game.
         */ 
        std::vector<SDL_Event> getLegalActions();

        /**
         * \fn long getFrameNumber()
         * \brief Returns the number of frames since the emulator has started.
         * \return The number of frames since the emulator has started.
         */ 
        long getFrameNumber();

        /**
         * \fn CPCScreen getRGBScreen()
         * \brief Returns a CPCScreen object corresponding to the current displayed pixels.
         * \return A CPCScreen object corresponding to the current displayed pixels.
         */ 
        CPCScreen getRGBScreen();

        /**
         * \fn void setEmulatorSpeed(int speed)
         * \brief Sets the speed of the emulator given a percentage of its normal speed.
         * speed should be in [MIN_SPEED_SETTING, MAX_SPEED_SETTING].
         * \param speed: The percentage of the normal speed that should be used.
         */
        void setEmulatorSpeed(int speed);

        /**
         * \fn bool pokeMemory(unsigned int addr, int value)
         * \brief Pokes the given address in memory with the given value.
         * \param addr: the address to poke. This address should be in range [0, 65535].
         * \param value: The value to set the address to. This value should be in range [-128, 255].
         * \return True if the value was correctly poked, false otherwise.
         */
        bool pokeMemory(unsigned int addr, int value);

        /**
         * \fn void toggleSound(bool value)
         * \brief Enables or disables the sound.
         * \param value: If true the sound will be enable, it won't otherwise. Sound is enabled by 
         * default.
         */
        void toggleSound(bool value);

        /**
         * \fn void step()
         * \brief Applies one step to the emulator. All of the code of this method is shamelessly copied from 
         * the cap32.cpp file from the Caprice32 emulator source code.
         */ 
        void step();

    private:
        void fillLegalActions();
        void loadGameData(SupportedGames game, std::string pathToData);

        int iExitCondition; /**< From cap32.cpp. */
        bool bolDone; /**< From cap32.cpp. */
        bool take_screenshot; /**< From cap32.cpp. */
        SDL_Event event; /**< From cap32.cpp. */
        std::vector<std::string> slot_list; /**< From cap32.cpp. */

        int previousScore; /**< The score before the next step is executed. */
        long numberFrame; /**< The number of frames since the start of the game. */
        SupportedGames currentSupportedGame; /**< The current game that is being played. */
        std::string pathtoCurrentGame; /**< The path to the current game's file. */

        std::string pathToData; /**< This path should grant acces to directories of the library. */

        GameInfo currentGame; /**< Information about the current game. */
};

#endif  // __AMLE_INTERFACE_H__