#include "TestAmle.h"
#include "SDL/SDL.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "stdio.h"
#include "unistd.h"
#include <fcntl.h>

// Output macros
#ifdef _WIN32
#define NULL_DEVICE "NUL:"
#else
#define NULL_DEVICE "/dev/null"
#endif

// General macros
#define MIN_ADDRESS 0
#define MAX_ADDRESS 65536
#define MIN_VALUE 0x00
#define MAX_VALUE 0x100

#define MIN_LIVES_ARKANOID 0
#define MAX_LIVES_ARKANOID 10
#define MIN_SCORE_ARKANOID 0x30
#define MAX_SCORE_ARKANOID 0x40
#define ARKANOID_SNA_PATH "snap/arka.sna"
#define BUGGY_SNA_PATH "snap/buggy.sna"
#define ARKANOID_ROM_PATH "disk/arka.dsk"
#define ARKANOID_TEST_PATH "build/test.sna"

// Macros for legal action
#define EXPECTED_NB_ACTIONS_NO_GAME 0
#define EXPECTED_NB_ACTIONS_ARKANOID 10
#define EXPECTED_NB_ACTIONS_BUGGY 12

// Macros for nbLives 
#define DEFAULT_NUMBER_LIVES_ARKANOID 4
#define ARKANOID_LIVES_ADDRESS 0xb7d4

// Macros for reset game
#define AKANOID_SCORE_ADDRESS 0x6

// Macro for game over
#define BUGGY_GAMEOVER_ADDRESS 0x9674
#define BUGGY_GAMEOVER_VALUE 0

// Macro for get screen
#define SCREEN_BLUE 127
#define SCREEN_YELLOW 16776960

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestAmle);

void TestAmle::setUp() {
    srand(time(NULL));

    // Redirect cout
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;
    std::cout.rdbuf(NULL);

    // Redirect cerr
    std::streambuf* oldCerrStreamBuf = std::cerr.rdbuf();
    std::ostringstream strCerr;
    std::cerr.rdbuf(NULL);

    // Redirect stderr
    
    amle = new AmLEInterface();

    // Restore old cout.
    std::cout.rdbuf(oldCoutStreamBuf);

    // Restore old cerr
    std::cerr.rdbuf(oldCerrStreamBuf);

    amle->setEmulatorSpeed(MAX_SPEED_SETTING);
}


void  TestAmle::tearDown() {
    delete amle;
}

SDL_Event getRandomEvent(std::vector<SDL_Event> possibleEvents) {
    return possibleEvents[rand() % possibleEvents.size()];
}

std::string buildErrorMessage(const char * function, int line, std::string expected, std::string result) {
    std::string message = "In function " + std::string(function) + " at line " + std::to_string(line) + 
                          " expected " + expected + " but got " + result;
    return message;
}

// [low; high[
int getRandomInt(int low, int high) {
    return rand() % (high - low) + low;
}

int getRandomNbLives() {
    return getRandomInt(MIN_LIVES_ARKANOID, MAX_LIVES_ARKANOID);
}

int getRandomScore() {
    return getRandomInt(MIN_SCORE_ARKANOID, MAX_SCORE_ARKANOID);
}

int getElapsedTime(std::chrono::time_point<std::chrono::system_clock> prevTime) {
    std::chrono::time_point<std::chrono::system_clock> currTime = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currTime-prevTime).count();
}

void TestAmle::testPokeMemory() {
    // The test for this method does NOT rely on any other method of the AmLE
    // Therefore, if this tests works, it is a reliable method to use in other tests
    // This is important because thanks to this, a test that only uses this method will then 
    // be a reliable test too, and then it's basically the butterfly effect with the same 
    // reasoning 
    //
    // Note: This only means that tests are reliable, not that they are exhaustive
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);

    for(unsigned int i = 0; i < MAX_ADDRESS; i++) {
        unsigned int address = (unsigned int)getRandomInt(MIN_ADDRESS, MAX_ADDRESS);
        int value = getRandomInt(MIN_VALUE, MAX_VALUE);
        amle->pokeMemory(address, value);

        // This method is only a public duplicate of an inline method of Caprice32
        int readValue = (int)amle_public_read_mem(address);
        message = buildErrorMessage(__func__, __LINE__, std::to_string(value), std::to_string(readValue));
        CPPUNIT_ASSERT_EQUAL_MESSAGE(message, value, readValue);
    }
}

void TestAmle::testSaveAndLoadSnapshot() {
    int lives = getRandomNbLives();
    int scoreHex = getRandomScore(); 
    int scoreDec = (scoreHex - ASCII_SHIFT) * 100000;

    // Create a snapshot in which the player should have a random score and number of lives
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    amle->pokeMemory(ARKANOID_LIVES_ADDRESS, lives);
    amle->pokeMemory(AKANOID_SCORE_ADDRESS, scoreHex);
    amle->saveSnapshot(ARKANOID_TEST_PATH);

    // Reload the snapshot and check if the values are correct
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_TEST_PATH);

    message = buildErrorMessage(__func__, __LINE__, std::to_string(lives), std::to_string(amle->getNbLives()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, lives, amle->getNbLives());

    message = buildErrorMessage(__func__, __LINE__, std::to_string(scoreDec), std::to_string(amle->getScore()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, scoreDec, amle->getScore());
}

void TestAmle::testLegalActions() {
    int size;

    // No game loaded should return a number of no possible action
    size = amle->getLegalActions().size();
    message = buildErrorMessage(__func__, __LINE__, std::to_string(EXPECTED_NB_ACTIONS_NO_GAME), std::to_string(size));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, EXPECTED_NB_ACTIONS_NO_GAME, size);

    // Arkanoid should give 10 possible actions
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    size = amle->getLegalActions().size();
    message = buildErrorMessage(__func__, __LINE__, std::to_string(EXPECTED_NB_ACTIONS_ARKANOID), std::to_string(size));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, EXPECTED_NB_ACTIONS_ARKANOID, size);

    // Buggy Boy should give 12 possible actions
    amle->loadSnapshot(SupportedGames::Buggy, BUGGY_SNA_PATH);
    size = amle->getLegalActions().size();
    message = buildErrorMessage(__func__, __LINE__, std::to_string(EXPECTED_NB_ACTIONS_BUGGY), std::to_string(size));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, EXPECTED_NB_ACTIONS_BUGGY, size);

}

void TestAmle::testNbLives() {
    int nbLives;

    // At the start of the game, the player should have 4 lives
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    nbLives = amle->getNbLives();
    message = buildErrorMessage(__func__, __LINE__, std::to_string(DEFAULT_NUMBER_LIVES_ARKANOID), std::to_string(nbLives));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, DEFAULT_NUMBER_LIVES_ARKANOID, nbLives);

    // Set the number of lives to a random number
    nbLives = getRandomNbLives();
    amle->pokeMemory(ARKANOID_LIVES_ADDRESS, nbLives);

    message = buildErrorMessage(__func__, __LINE__, std::to_string(MIN_LIVES_ARKANOID), std::to_string(nbLives));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, nbLives, amle->getNbLives());
}

void TestAmle::testResetGame() {
    int nbLivesBefore;
    int nbScoreBefore;

    // Get the base values of the given snapshot
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    nbLivesBefore = amle->getNbLives();
    nbScoreBefore = amle->getScore();

    // Change these values (% 4 to make sure the number of lives is less than 4
    // because we want a different number of lives and without this mod, it could be 4)
    // Same for the score but for the value 0
    amle->pokeMemory(ARKANOID_LIVES_ADDRESS, getRandomNbLives() % 4);
    amle->pokeMemory(AKANOID_SCORE_ADDRESS, getRandomScore() % 9 + 1);

    // Checking that they did change
    if(nbLivesBefore == amle->getNbLives()) {
        message = buildErrorMessage(__func__, __LINE__, "not " + std::to_string(nbLivesBefore), std::to_string(amle->getNbLives()));
        CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
    }

    if(nbScoreBefore == amle->getScore()) {
        message = buildErrorMessage(__func__, __LINE__, "not " + std::to_string(nbScoreBefore), std::to_string(amle->getScore()));
        CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
    }

    // Reset to get the original values back
    amle->resetGame();

    message = buildErrorMessage(__func__, __LINE__, std::to_string(nbLivesBefore), std::to_string(amle->getNbLives()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, nbLivesBefore, amle->getNbLives());

    message = buildErrorMessage(__func__, __LINE__, std::to_string(nbScoreBefore), std::to_string(amle->getScore()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, nbScoreBefore, amle->getScore());
}

void TestAmle::testGameOver() {
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);

    // Actual game over
    amle->pokeMemory(ARKANOID_LIVES_ADDRESS, 0);
    message = buildErrorMessage(__func__, __LINE__, "1", std::to_string(amle->gameOver()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, amle->gameOver());

    // Not game over
    amle->pokeMemory(ARKANOID_LIVES_ADDRESS, getRandomNbLives() % 9 + 1);
    message = buildErrorMessage(__func__, __LINE__, "0", std::to_string(amle->gameOver()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, false, amle->gameOver());

    amle->loadSnapshot(SupportedGames::Buggy, BUGGY_SNA_PATH);

    // Actual game over
    amle->pokeMemory(BUGGY_GAMEOVER_ADDRESS, 0);
    message = buildErrorMessage(__func__, __LINE__, "1", std::to_string(amle->gameOver()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, amle->gameOver());

    // Not game over
    amle->pokeMemory(BUGGY_GAMEOVER_ADDRESS, getRandomNbLives() % 9 + 1);
    message = buildErrorMessage(__func__, __LINE__, "0", std::to_string(amle->gameOver()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, false, amle->gameOver());
}

void TestAmle::testStep() {
    int oldNumberLives;
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    oldNumberLives = amle->getNbLives();

    while(!amle->gameOver()) {
        amle->step();

        // Making sure the number of lives is continuous
        if(oldNumberLives != amle->getNbLives() && oldNumberLives != amle->getNbLives() - 1 
                                                && oldNumberLives != amle->getNbLives() + 1) {

            std::string expected = "(" + std::to_string(oldNumberLives) + ", " + 
                                   std::to_string(oldNumberLives - 1) + ", " + 
                                   std::to_string(oldNumberLives + 1) + ")";

            message = buildErrorMessage(__func__, __LINE__, expected, std::to_string(amle->getNbLives()));
            CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
        }
        
        oldNumberLives = amle->getNbLives();
    }
}

void TestAmle::testEmulatorSpeed() {
    int fastTime;
    int slowTime;

    // Get elapsed time with max speed
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    timer = std::chrono::system_clock::now();
    amle->setEmulatorSpeed(MAX_SPEED_SETTING);
    while(!amle->gameOver()) {
        amle->step();
    }
    fastTime = getElapsedTime(timer);

    // Get elapsed time with half the max speed
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    timer = std::chrono::system_clock::now();
    amle->setEmulatorSpeed(MAX_SPEED_SETTING / 2);
    while(!amle->gameOver()) {
        amle->step();
    }
    slowTime = getElapsedTime(timer);

    // Max speed should be faster, therefore if the elapsed time of the max speed 
    // is higher, it is a problem
    if(slowTime <= fastTime) {
        message = buildErrorMessage(__func__, __LINE__, "slowTime <= fastTime", "fastTime >= slowTime");
        CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
    }
}

void TestAmle::testGetFrameNumber() {
    long oldFrameNumber;
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    oldFrameNumber = amle->getFrameNumber();

    // The number of frame should always be one more than the previous number of frame
    while(!amle->gameOver()) {
        amle->step();
        message = buildErrorMessage(__func__, __LINE__, std::to_string(oldFrameNumber + 1), std::to_string(amle->getFrameNumber()));
        CPPUNIT_ASSERT_EQUAL_MESSAGE(message, oldFrameNumber + 1, amle->getFrameNumber());
        oldFrameNumber = amle->getFrameNumber();
    }
}

void TestAmle::testGetRGBScreen() {

    // Wait for the end of the init
    for(unsigned int i = 0; i <  100000; i++) {
        amle->step();
    }

    CPCScreen screen = amle->getRGBScreen();

    // On the main screen of Caprice32, there are only two colors
    for(unsigned int i = 0; i < screen.pixels.size(); i++) {
        if(screen.pixels[i] != SCREEN_BLUE && screen.pixels[i] != SCREEN_YELLOW) {
            message = buildErrorMessage(__func__, __LINE__, "(" + std::to_string(SCREEN_YELLOW) + " , " + 
                                                            std::to_string(SCREEN_BLUE) + ")", std::to_string(screen.pixels[i]));
            CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
        }
    }
}

void TestAmle::testToggleSound() {

    // Test if disabling sound works
    amle->toggleSound(false);
    message = buildErrorMessage(__func__, __LINE__, "0", std::to_string((bool)CPC.snd_enabled));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, false, (bool)CPC.snd_enabled);

    // Test if enabling sound works
    amle->toggleSound(true);
    message = buildErrorMessage(__func__, __LINE__, "1", std::to_string((bool)CPC.snd_enabled));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, (bool)CPC.snd_enabled);
}

void TestAmle::testAct() {
    int fastTime;
    int slowTime;

    // Get elapsed time without doing anything
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    timer = std::chrono::system_clock::now();
    while(!amle->gameOver()) {
        amle->step();
    }
    slowTime = getElapsedTime(timer);

    // Get elapsed time when spamming space
    amle->loadSnapshot(SupportedGames::Arkanoid, ARKANOID_SNA_PATH);
    std::vector<SDL_Event> possible_actions = amle->getLegalActions();
    timer = std::chrono::system_clock::now();
    while(!amle->gameOver()) {
        // Spamming space should be faster because otherwise the ball sticks to the 
        // bar for a few frames
        SDL_Event action = possible_actions[0];
        amle->act(&action);
        amle->step();
    }
    fastTime = getElapsedTime(timer);

    if(slowTime <= fastTime) {
        message = buildErrorMessage(__func__, __LINE__, "slowTime <= fastTime", "fastTime >= slowTime");
        CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(message, "");
    }
}