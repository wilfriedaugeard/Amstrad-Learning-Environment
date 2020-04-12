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
 *  SupportedGamesUtils.h
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef SUPPORTEDGAMESUTILS_H_
#define SUPPORTEDGAMESUTILS_H_

/** \file SupportedGamesUtils.h
 * \brief This file is used to read and interpret .data files. These files are used to store 
 * memory addresses and meta information about games. 
 * 
 * .data file are constructed this way :
 * 
 * <key>=<value1>-<value2>-...
 * 
 * <key>=<value1>-<value2>-...
 */ 

#include "GameInfo.h"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

// Note: These two next MACROS are obviously not useful from a coding perspective (an ASCII character is 
// obviously of length 1). These are used to make the code more readable.
#define SKIP_EQUAL 1 /**< Size of the ASCII assign character in a .data file. */
#define SKIP_DASH 1 /**< Size of the ASCII separator character in a .data file. */
#define SEPARATOR "-" /**< The separator used in a .data file. */

/**
 * This key is used to store the name of the game in a .data file.
 */ 
#define NAME_KEY "name"

/**
 * This key is used to store the address of the lives of a game in a .data file.
 */ 
#define LIVES_KEY "lives"

/** 
 * This key is used to store the address of the game over of a game in a .data file. 
 * If the value found has to be compared, it will be interpreted as an hex value. For more info, see StorageType.
 */
#define GAME_OVER_HEX "hexgameover" 

/** 
 * This key is used to store the address of the game over of a game in a .data file. 
 * If the value found has to be compared, it will be interpreted as an decimal value. For more info, see StorageType.
 */
#define GAME_OVER_DEC "decgameover"  

/** 
 * This key is used to store the address of the game over of a game in a .data file. 
 * If the value found has to be compared, it will be interpreted as an ascii value. For more info, see StorageType.
 */
#define GAME_OVER_ASCII "asciigameover"

/** 
 * This key is used to store the address of the score of a game in a .data file. 
 * It will be interpreted as an hex value. For more info, see StorageType.
 */
#define SCORE_HEX "hexscore"

/** 
 * This key is used to store the address of the score of a game in a .data file. 
 * It will be interpreted as an decimal value. For more info, see StorageType.
 */
#define SCORE_DEC "decscore"

/** 
 * This key is used to store the address of the score of a game in a .data file. 
 * It will be interpreted as an ascii value. For more info, see StorageType.
 */
#define SCORE_ASCII "asciiscore"

/**
 * This is the key used to store all the legal actions. Legal actions should be passed as the actual character to use.
 * If this is not possible, there are special keyword, see ACTION_RETURN, ACTION_DELETE and ACTION_BACKSLASH. Also, 
 * the text editor used might influence the existence of spaces at the end of the file. An easy fix is to make sure 
 * the last character of the file is not " " while representing a legal action.
 */ 
#define LEGAL_ACTIONS_KEY "legalactions"

/**
 * The return (\n) character in a .data file is represented by the keyword "return".
 */
#define ACTION_RETURN "return"

/**
 * The delete (\r) character in a .data file is represented by the keyword "delete".
 */
#define ACTION_DELETE "delete"

/**
 * The backslash (\\) character in a .data file is represented by the keyword "backslash".
 */
#define ACTION_BACKSLASH "backslash"

/**
 * The path to the arkanoid.data file. This is used to load all important addresses corresponding to the Arkanoid game.
 */
#define ARKANOID_DATA_PATH "SupportedGames/arkanoid.data"

/**
 * The path to the buggy.data file. This is used to load all important addresses corresponding to the Buggy Boy game.
 */
#define BUGGY_DATA_PATH "SupportedGames/buggy.data"

/**
 * All the games in this enum are the games supported by the Amstrad Learning Environment. If a game is not in this enum 
 * it is NOT supported. Games can be added, a guide might be added when more important stuff is done. If this is crucial 
 * to add games, please contact us.
 */ 
enum SupportedGames {
    Arkanoid, /**< Arkanoid II. */
    Buggy /**< Buggy Boy. */
};

/**
 * \fn GameInfo getGameFromEnum(SupportedGames game)
 * \brief This function returns all the info read in the corresponding .data file as a GameInfo object.
 * \param game: The enum object SupportedGames corresponding to the game loaded. This is the USER responsibility to give the 
 * correct enum value. For instance, if game == Arkanoid but Buggy Boy is loaded, the behaviour is UNDEFINED. Everything should 
 * run fine but values returned by the GameInfo object will be corrupted and/or incorrect.
 * \param pathToData: The path to the data folder in the library.
 * \return The GameInfo object containing all the addresses and meta data found in the .data file.
 */
GameInfo getGameFromEnum(SupportedGames game, std::string pathtoData);

#endif