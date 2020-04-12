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
 *  GameInfo.h
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef GAMEINFO_H_
#define GAMEINFO_H_

/**
 * \file GameInfo.h
 * \brief Ths file contains a class that represents all the important information a game can 
 * contain.
 */ 

#include <string>
#include <vector>
#include "AddressRange.h"

#include "StorageType.h"

#include "SDL/SDL.h"

/**
 * A class containing all the important information that could be fetched. This class is meant to be generic 
 * therefore it can not hold a lot of information otherwise it would be too inaccurate for a lot games.
 * 
 * This class contains : 
 * 
 * - The name of the game
 * - The current number of lives (if any)
 * - The current score (if any)
 * - The Expected game over value (if any)
 * - The memory addresses of all the above information (except the name)
 * - The list of legal actions
 */ 
class GameInfo {
    public:

        /**
         * \fn GameInfo()
         * \brief Default constructor, everything will be initialized to 0, empty etc.
         */ 
        GameInfo();

        /**
         * Default desctructor.
         */ 
        ~GameInfo();

        /**
         * \fn std::string getName()
         * \brief Retuns the name of the current game.
         * \return The name of the current game.
         */ 
        std::string getName();

        /**
         * \fn void setName(std::string name)
         * \brief Sets the name of the current game.
         * \param name: The name to set.
         */ 
        void setName(std::string name);

        /**
         * \fn int getLivesAddress()
         * \brief Returns the address of the number of lives.
         * \return The address of the number of lives.
         */ 
        int getLivesAddress();

        /**
         * \fn void setLivesAddress(int addr)
         * \brief Sets the address of the number of lives. 
         * \param addr: The addres to set.
         */ 
        void setLivesAddress(int addr);

        /**
         * \fn AddressRange getGameOverAddressRange()
         * \brief Returns the address range of the game over value.
         * \return The address range of the game over value.
         */ 
        AddressRange getGameOverAddressRange();

        /**
         * \fn void setGameOverAddressRange(AddressRange addr)
         * \brief Sets the address range of the game over value.
         * \param addr: The address range to set.
         */ 
        void setGameOverAddressRange(AddressRange addr);

        /**
         * \fn int getGameOverExpectedValue()
         * \brief Returns the expected value when a game over happens.
         * \return The expected value when a game over happens.
         */ 
        int getGameOverExpectedValue();

        /**
         * \fn void setGameOverExpectedValue(int value)
         * \brief Sets the expected value when a game over happens.
         * \param value: The expected value to set.
         */ 
        void setGameOverExpectedValue(int value);

        /**
         * \fn AddressRange getScoreAddressRange()
         * \brief Returns the address range of the score.
         * \return The addres range of the score.
         */ 
        AddressRange getScoreAddressRange();

        /**
         * \fn void setScoreAddressRange(AddressRange range)
         * \brief Sets the address range of the score.
         * \param range: The range to set.
         */ 
        void setScoreAddressRange(AddressRange range);

        /**
         * \fn StorageType getScoreType()
         * \brief Returns the type used to encode the score. More info StorageType.
         * \return The type used to encode the score.
         */ 
        StorageType getScoreType();

        /**
         * \fn void setScoreType(StorageType type)
         * \brief Sets the type used to encode the score. More info StorageType.
         * \param type: The StorageType to set.
         */ 
        void setScoreType(StorageType type);

        /**
         * \fn StorageType getGameOverValueType()
         * \brief Returns the type used to encode the game over value. More info StorageType.
         * \return The type used to encode the game over value.
         */ 
        StorageType getGameOverValueType();

        /**
         * \fn void setGameOverValueType(StorageType type)
         * \brief Sets the type used to encode the game over value. More info StorageType.
         * \param type: The StorageType to set.
         */ 
        void setGameOverValueType(StorageType type);

        /**
         * \fn std::vector<char> getLegalActionsAsChars()
         * \brief Returns the list of available actions in the game as characters.
         * \return A vector of characters corresponding of the available actions.
         */ 
        std::vector<char> getLegalActionsAsChars();

        /**
         * \fn void addLegalActionAsChar(char action)
         * \brief Add a new legal action to the list. If the action is already in the 
         * list, it will be added again. This should NOT cause any issues but this 
         * could be responsible for some time loss.
         *
         * The option to add twice the same action (or more) is kept in case the 
         * likelihood of using a certain action should be enhanced.
         * \param action: The action to add.
         */ 
        void addLegalActionAsChar(char action);

        /**
         * \fn std::vector<SDL_Event> getLegalActionsAsEvents()
         * \brief Returns the list of available actions in the game as SDL_Events because this 
         * is the type of events that should be pushed back to simulate inputs.
         * \return A vector of SDL_Events corresponding to the available actions.
         */ 
        std::vector<SDL_Event> getLegalActionsAsEvents();

        /**
         * \fn void addLegalActionAsEvent(SDL_Event event)
         * \brief Add a new legal action to the list. If the action is already in the 
         * list, it will be added again. This should NOT cause any issues but this 
         * could be responsible for some time loss.
         *
         * The option to add twice the same action (or more) is kept in case the 
         * likelihood of using a certain action should be enhanced.
         * \param event: The action to add.
         */ 
        void addLegalActionAsEvent(SDL_Event event);

    private:
        std::string name; /**< The name of the current game. */

        int livesAddress; /**< The memory address of the lives (if any). */
        AddressRange gameOverAddress; /**< The memory address range of the game over (if any). */
        AddressRange score; /**< The memory address range of the score (if any). */

        int gameOverExpectedValue; /**< The expected game over value to be found if a game over happens. */

        StorageType scoreType; /**< The encoding type in memory of the score. More info StorageType. */
        StorageType gameOverValueType; /**< The encoding type in memory of the game over value. More info StorageType. */

        std::vector<char> legalActionsAsChars; /**< The list of all legal actions as characters. */
        std::vector<SDL_Event> legalActionsAsEvents; /**< The list of all legal actions as SDL_Events. */
};

#endif