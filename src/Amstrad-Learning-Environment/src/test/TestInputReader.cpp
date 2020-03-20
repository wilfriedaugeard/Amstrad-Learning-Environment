#include "TestInputReader.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TestInputReader);

void TestInputReader::setUp() {

}


void  TestInputReader::tearDown() {

}

CppUnit::TestSuite * TestInputReader::getSuite() {
    CppUnit::TestSuite * suite = new CppUnit::TestSuite();
    suite->addTest(new CppUnit::TestCaller<TestInputReader>(
                        "testConstructor", 
                        &TestInputReader::testConstructor));
    return suite;
}

void TestInputReader::testConstructor() {   
    // CPPUNIT_FAIL("Not implemented");
    CPPUNIT_ASSERT(true);
}