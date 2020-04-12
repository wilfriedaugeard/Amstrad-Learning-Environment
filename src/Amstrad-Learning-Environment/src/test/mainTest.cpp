#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>


int main(int argc, char* argv[]) {

    std::cout << "================================" << std::endl;
    std::cout << "         TEST CPPUNIT" << std::endl;
    std::cout << "================================\n" << std::endl;

    // Adds the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());

    // Change the default outputter to a compiler error format outputter
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
    // Run the tests.
    runner.run();

    // Return error code 1 if the one of test failed.
    return 0;
}
