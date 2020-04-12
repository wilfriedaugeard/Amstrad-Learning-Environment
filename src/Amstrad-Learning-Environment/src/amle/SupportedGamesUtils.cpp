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
 *  SupportedGamesUtils.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "SupportedGamesUtils.h"

char stringToChar(std::string action) {
    // Special characters handling
    if(action.compare(ACTION_RETURN) == 0) {
        return '\n';
    }

    if(action.compare(ACTION_DELETE) == 0) {
        return '\b';
    }

    if(action.compare(ACTION_BACKSLASH) == 0) {
        return '\\';
    }

    // Otherwise, return the read character
    return action.at(0);
}

AddressRange getAddressRangeFromString(std::string * value) {
    // Extract the two components of the address range of the given line
    int index = value->find(SEPARATOR);

    AddressRange range;
    std::stringstream ss;
    ss << std::hex << value->substr(0, index);
    ss >> range.start;

    *value = value->substr(index + SKIP_DASH, value->size());
    index = value->find(SEPARATOR);
    
    ss.clear();
    ss << std::hex << value->substr(0, index);
    ss >> range.end;

    *value = value->substr(index + SKIP_DASH, value->size());

    return range;
}

int hexStringToInt(std::string str) {
    int x;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return x;
}

void readKeyValue(GameInfo * game, std::string line, std::string key) {
    int start = key.size() + SKIP_EQUAL;
    int end = line.size();
    std::string value = line.substr(start, end);

    // A switch butmore readable as a list of ifs that interpret the keys
    // For more info, read the .h possible keys description
    if(key.compare(NAME_KEY) == 0) {
        game->setName(std::string(value));
    }

    if(key.compare(LIVES_KEY) == 0) {
        game->setLivesAddress(hexStringToInt(value));
    }

    if(key.compare(GAME_OVER_HEX) == 0 || key.compare(GAME_OVER_DEC) == 0 || key.compare(GAME_OVER_ASCII) == 0) {
        AddressRange range = getAddressRangeFromString(&value); 
        game->setGameOverAddressRange(range);
        game->setGameOverExpectedValue(hexStringToInt(value));

        if(key.compare(GAME_OVER_HEX) == 0) {
            game->setGameOverValueType(StorageType::HEX);
        }
        else if(key.compare(GAME_OVER_DEC) == 0) {
            game->setGameOverValueType(StorageType::DEC);
        }
        else if(key.compare(GAME_OVER_ASCII) == 0) {
            game->setGameOverValueType(StorageType::ASCII);
        }
    }

    if(key.compare(SCORE_HEX) == 0 || key.compare(SCORE_DEC) == 0 || key.compare(SCORE_ASCII) == 0) {
        AddressRange range = getAddressRangeFromString(&value);

        if(key.compare(SCORE_HEX) == 0) {
            game->setScoreType(StorageType::HEX);
        }
        else if(key.compare(SCORE_DEC) == 0) {
            game->setScoreType(StorageType::DEC);
        }
        else if(key.compare(SCORE_ASCII) == 0) {
            game->setScoreType(StorageType::ASCII);
        }

        game->setScoreAddressRange(range);
    }

    if(key.compare(LEGAL_ACTIONS_KEY) == 0) {
        int index = 0;
        while((index = value.find(SEPARATOR)) >= 0) {
            std::string action = (value.substr(0, index));
            char chrAction = stringToChar(action);
            value = value.substr(index + SKIP_DASH, value.size());
            game->addLegalActionAsChar(chrAction);
        }

        std::string action = (value.substr(0, value.size()));
        char chrAction = stringToChar(action);
        game->addLegalActionAsChar(chrAction);
    }
}

void getGameFromFile(GameInfo * game, std::string gamePath) {
    std::string line;

    // The list of key put into an array to improve code readability
    // This function is almost never called, therefore constructing it here 
    // is not a big deal
    std::vector<std::string> keys;
    keys.push_back(NAME_KEY);
    keys.push_back(LIVES_KEY);
    keys.push_back(GAME_OVER_HEX);
    keys.push_back(GAME_OVER_DEC);
    keys.push_back(GAME_OVER_ASCII);
    keys.push_back(SCORE_ASCII);
    keys.push_back(SCORE_HEX);
    keys.push_back(SCORE_DEC);
    keys.push_back(LEGAL_ACTIONS_KEY);

    std::ifstream file (gamePath);

    // Read the .data file line by line
    // One line should correspond to one key and its corresponding values
    if (file.is_open()) {
        while (getline(file, line)) {

            // Looks for keys in the line
            for(unsigned int i = 0; i < keys.size(); i++) {

                // If a key is found, interpret the line
                if(line.substr(0, keys[i].size()).compare(keys[i]) == 0) {
                    readKeyValue(game, line, keys[i]);
                }
            }
        }
        file.close();
    }
}

GameInfo getGameFromEnum(SupportedGames game, std::string pathToData) {
    GameInfo gameInfo;

    // Switch to read the correct .data file.
    // (not a switch though because it is more readable this way and performance is not an issue here)
    if(game == SupportedGames::Arkanoid) {
        getGameFromFile(&gameInfo, pathToData + ARKANOID_DATA_PATH);
    }

    if(game == SupportedGames::Buggy) {
        getGameFromFile(&gameInfo, pathToData + BUGGY_DATA_PATH);
    }
    
    return gameInfo;
}