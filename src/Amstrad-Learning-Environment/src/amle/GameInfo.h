#ifndef GAMEINFO_H_
#define GAMEINFO_H_

#include <string>
#include <vector>
#include "AddressRange.h"

#include "StorageType.h"

#include "SDL.h"

class GameInfo {
    public:
        GameInfo();
        ~GameInfo();

        std::string getName();
        void setName(std::string name);

        int getLivesAddress();
        void setLivesAddress(int addr);

        AddressRange getGameOverAddressRange();
        void setGameOverAddressRange(AddressRange addr);

        int getGameOverExpectedValue();
        void setGameOverExpectedValue(int value);

        AddressRange getScoreAddressRange();
        void setScoreAddressRange(int start, int end);
        void setScoreAddressRange(AddressRange range);

        /*
        bool isScoreAscii();
        void setIsScoreAscii(bool isAscii);
        */

        StorageType getScoreType();
        void setScoreType(StorageType type);

        StorageType getGameOverValueType();
        void setGameOverValueType(StorageType type);

        std::vector<char> getLegalActionsAsChars();
        void addLegalActionAsChar(char action);

        std::vector<SDL_Event> getLegalActionsAsEvents();
        void addLegalActionAsEvent(SDL_Event event);

    private:
        std::string name;
        int livesAddress;
        AddressRange gameOverAddress;
        int gameOverExpectedValue;
        AddressRange score;
        // bool scoreAscii;
        StorageType scoreType;
        StorageType gameOverValueType;
        std::vector<char> legalActionsAsChars;
        std::vector<SDL_Event> legalActionsAsEvents;
};

#endif