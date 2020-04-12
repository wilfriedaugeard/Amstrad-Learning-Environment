#ifndef _TESTAMLE_H_
#define _TESTAMLE_H_

#include <cppunit/extensions/HelperMacros.h>
#include "amle_interface.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>

class TestAmle : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestAmle);
    
    CPPUNIT_TEST(testPokeMemory);
    CPPUNIT_TEST(testSaveAndLoadSnapshot);
    CPPUNIT_TEST(testLegalActions);
    CPPUNIT_TEST(testNbLives);
    CPPUNIT_TEST(testResetGame);
    CPPUNIT_TEST(testGameOver);
    CPPUNIT_TEST(testStep);
    CPPUNIT_TEST(testEmulatorSpeed);
    CPPUNIT_TEST(testGetFrameNumber);
    CPPUNIT_TEST(testGetRGBScreen);
    CPPUNIT_TEST(testToggleSound);
    CPPUNIT_TEST(testAct);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        void testPokeMemory();
        void testSaveAndLoadSnapshot();
        void testLegalActions();
        void testNbLives();
        void testResetGame();
        void testGameOver();
        void testStep();
        void testEmulatorSpeed();
        void testGetFrameNumber();
        void testGetRGBScreen();
        void testToggleSound();
        void testAct();

    private:
        AmLEInterface* amle;
        std::string message;
        std::chrono::time_point<std::chrono::system_clock> timer;
};




#endif