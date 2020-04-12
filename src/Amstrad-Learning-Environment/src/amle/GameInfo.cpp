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
 *  GameInfo.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "GameInfo.h"
#include <iostream>

GameInfo::GameInfo() {

}

GameInfo::~GameInfo() {

}

std::string GameInfo::getName() {
    return name;
}

void GameInfo::setName(std::string name) {
    this->name = name;
}

int GameInfo::getLivesAddress() {
    return this->livesAddress;
}

void GameInfo::setLivesAddress(int addr) {
    this->livesAddress = addr;
}

AddressRange GameInfo::getGameOverAddressRange() {
    return this->gameOverAddress;
}

void GameInfo::setGameOverAddressRange(AddressRange addr) {
    this->gameOverAddress = addr;
}

int GameInfo::getGameOverExpectedValue() {
    return this->gameOverExpectedValue;
}

void GameInfo::setGameOverExpectedValue(int value) {
    this->gameOverExpectedValue = value;
}

AddressRange GameInfo::getScoreAddressRange() {
    return this->score;
}

void GameInfo::setScoreAddressRange(AddressRange range) {
    this->score = range;
}

StorageType GameInfo::getScoreType() {
    return this->scoreType;
}

void GameInfo::setScoreType(StorageType type) {
    this->scoreType = type;
}

StorageType GameInfo::getGameOverValueType() {
    return this->gameOverValueType;
}

void GameInfo::setGameOverValueType(StorageType type) {
    this->gameOverValueType = type;
}

std::vector<char> GameInfo::getLegalActionsAsChars() {
    return this->legalActionsAsChars;
}
void GameInfo::addLegalActionAsChar(char action) {
    this->legalActionsAsChars.push_back(action);
}

std::vector<SDL_Event> GameInfo::getLegalActionsAsEvents() {
    return this->legalActionsAsEvents;
}

void GameInfo::addLegalActionAsEvent(SDL_Event event) {
    this->legalActionsAsEvents.push_back(event);
}