#include "SupportedGamesUtils.h"

char stringToChar(std::string action) {
    if(action.compare(ACTION_RETURN) == 0) {
        return '\n';
    }

    if(action.compare(ACTION_DELETE) == 0) {
        return '\b';
    }

    if(action.compare(ACTION_BACKSLASH) == 0) {
        return '\\';
    }
    return action.at(0);
}

AddressRange getAddressRangeFromString(std::string * value) {
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

void getGameFromFile(GameInfo * game, const char * gamePath) {
    std::string line;

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

    if (file.is_open()) {
        while (getline(file, line)) {
            for(unsigned int i = 0; i < keys.size(); i++) {
                if(line.substr(0, keys[i].size()).compare(keys[i]) == 0) {
                    readKeyValue(game, line, keys[i]);
                }
            }
        }
        file.close();
    }
}

GameInfo getGameFromEnum(SupportedGames game) {
    GameInfo gameInfo;

    if(game == SupportedGames::Arkanoid) {
        getGameFromFile(&gameInfo, ARKANOID_PATH);
    }

    if(game == SupportedGames::Buggy) {
        getGameFromFile(&gameInfo, BUGGY_PATH);
    }
    
    return gameInfo;
}