#include "GameInfo.h"

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
void GameInfo::setScoreAddressRange(int start, int end) {
    this->score.start = start;
    this->score.end = end;
}

void GameInfo::setScoreAddressRange(AddressRange range) {
    this->score = range;
}

/*
bool GameInfo::isScoreAscii() {
    return this->scoreAscii;
}

void GameInfo::setIsScoreAscii(bool isAscii) {
    this->scoreAscii = isAscii;
}
*/

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