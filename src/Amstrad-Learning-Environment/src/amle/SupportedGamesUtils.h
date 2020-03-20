#ifndef SUPPORTEDGAMESUTILS_H_
#define SUPPORTEDGAMESUTILS_H_

#include "GameInfo.h"
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#define SKIP_EQUAL 1
#define SKIP_DASH 1
#define SEPARATOR "-"

#define NAME_KEY "name"
#define LIVES_KEY "lives"
#define GAME_OVER_HEX "hexgameover"
#define GAME_OVER_DEC "decgameover"
#define GAME_OVER_ASCII "asciigameover"
#define SCORE_HEX "hexscore"
#define SCORE_DEC "decscore"
#define SCORE_ASCII "asciiscore"
#define LEGAL_ACTIONS_KEY "legalactions"

#define ACTION_RETURN "return"
#define ACTION_DELETE "delete"
#define ACTION_BACKSLASH "backslash"

#define ARKANOID_PATH "SupportedGames/arkanoid.data"
#define BUGGY_PATH "SupportedGames/buggy.data"

#include "AddressRange.h"

enum SupportedGames {
    Arkanoid,
    Buggy
};

GameInfo getGameFromEnum(SupportedGames game);

#endif