#ifndef __TESTINPUTREADER_H_
#define __TESTINPUTREADER_H_

#include <cppunit/extensions/HelperMacros.h>

class TestInputReader : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TestInputReader);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();
        static CppUnit::TestSuite * getSuite();
        void testConstructor();
};

#endif