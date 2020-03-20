// #include <cppunit/config/SourcePrefix.h>
// #include "TestInputReader.h"
// #include "InputReader.h"

// #include <thread>
// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>

// CPPUNIT_TEST_SUITE_REGISTRATION(TestInputReader);

// TestInputReader::TestInputReader(){}
// TestInputReader::~TestInputReader(){}

// void TestInputReader::setUp()
// {
//     amle_input_reader = new InputReader();
//     value = 0;
// }

// void TestInputReader::tearDown()
// {
//     delete amle_input_reader;
// }

// CppUnit::TestSuite * TestInputReader::createTestSuite() {
//     CppUnit::TestSuite * suite = new CppUnit::TestSuite();
//     suite->addTest(new CppUnit::TestCaller<TestInputReader>(
//                         "testDisplayInputs", 
//                         &TestInputReader::testDisplayInputs));
//     return suite;
// }

// void event(InputReader* amle_input_reader) {
//     amle_input_reader->displayInputs();  
// }

// void TestInputReader::testDisplayInputs() {
//     // Dummy test

//     // //Redirection
//     // dup(fileno(stdout));
//     // int pipefd[2];
//     // pipe2(pipefd, 0);
//     // // int dupRet = dup2(pipefd[1], fileno(stdout));

//     // //Set reading value and thread displayInputs method
//     // amle_input_reader->setReading(true);
    
//     // std::thread t(event, amle_input_reader);
    
//     // //Event to test
//     // SDL_Event sdlevent;
//     // sdlevent.type = SDL_KEYDOWN;
//     // sdlevent.key.keysym.sym = SDLK_0;
//     // SDL_PushEvent(&sdlevent);
//     // SDL_Delay(2000);

//     // //Stop the thread
//     // amle_input_reader->setReading(false);  
//     // //Close redirection
//     // // fflush(stdout);
//     // // close(pipefd[1]);
//     // // dup2(stdout_bk, fileno(stdout));//restore
//     // char buf[1024];
//     // read(pipefd[0], buf, 1024);

//     // t.join();
//     // // if()
//     CPPUNIT_ASSERT(false);//SDL_GetKeyName(sdlevent.key.keysym.sym),buf);
// }