cmake_minimum_required (VERSION 2.6)
project(amle)
set(AMLEVERSION "0.1.0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUNIT_CFLAGS} -std=c++11 -fPIC -g -fomit-frame-pointer -D__STDC_CONSTANT_MACROS")

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused")
endif()

# Some relative locations (self explanatory)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_DIR ${SOURCE_DIR}/test)
set(BUILD_DIR ${SOURCE_DIR}/build)

# The list of directories containing c/cpp files
##############################################################################
# IMPORTANT : If you create a new directory, please make sure to add it here !
##############################################################################
set(MODULES common controllers emucore environment games external external test amle)

# Finds all the c/cpp files from the different directories added above and ads them all a
# variable SOURCES. Used below when adding a new executable
foreach(module ${MODULES})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c)
  list(APPEND SOURCES ${module_sources})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c?[xp])
  list(APPEND SOURCES ${module_sources})
endforeach(module ${MODULES})

# Handles SDL and OS dependant libraries
add_definitions(-D__USE_SDL)
add_definitions(-DSOUND_SUPPORT)
find_package(SDL)
if(SDL_FOUND AND ${SDL_VERSION_STRING} VERSION_LESS 2)
  include_directories(${SDL_INCLUDE_DIR})
  list(APPEND LINK_LIBS ${SDL_LIBRARY} ${SDL_MAIN_LIBRARY})
else()
  MESSAGE("SDL 1.2 not found: You may need to manually edit CMakeLists.txt or run \"cmake -i\" to specify your SDL path.")
  # Uncomment below to specify the path to your SDL library and header file. Run "locate libSDL" and "locate SDL.h" if unsure.
  # link_directories(path_to_your_SDL)
  # include_directories(path_to_your_SDL_header)
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Cocoa")
    list(APPEND LINK_LIBS sdl sdlmain)
  else()
    list(APPEND LINK_LIBS SDL)
  endif()
endif()

# The list of directories to check to find all the .h files
##############################################################################
# IMPORTANT : If you create a new directory, please make sure to add it here !
##############################################################################
include_directories(
  ${SOURCE_DIR}
  ${SOURCE_DIR}/common
  ${SOURCE_DIR}/controllers
  ${SOURCE_DIR}/emucore
  ${SOURCE_DIR}/emucore/gui
  ${SOURCE_DIR}/environment
  ${SOURCE_DIR}/games
  ${SOURCE_DIR}/os_dependent
  ${SOURCE_DIR}/external
  ${SOURCE_DIR}/amle
  ${SOURCE_DIR}/amle_py
  ${SOURCE_DIR}/test
  /usr/include/freetype2
  usr/include/cppunit
)

# Main (and only) library of the project, we compile it here so we don't have to compile
# it for each executable
add_library(libamle ${SOURCES})

# Create the main executable from the project (although we will hopefully only
# be dispatching a library, this is useful to check everything is working)
add_executable(amle-bin ${SOURCE_DIR}/main.cpp)
set_target_properties(amle-bin PROPERTIES OUTPUT_NAME amle-exec)
set_target_properties(amle-bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(amle-bin libamle ${LINK_LIBS} z png freetype cppunit)

# Create an executable that runs tests which will surely make sure our project is flawless
add_executable(amle-tests-bin ${TEST_DIR}/mainTest.cpp)
set_target_properties(amle-tests-bin PROPERTIES OUTPUT_NAME amle-tests)
set_target_properties(amle-tests-bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(amle-tests-bin libamle ${LINK_LIBS} z png freetype cppunit)

# This target runs valgrind on all the created executable, the results
# can be found in the build directory with an appropriate name
#
# Note : There are already memory leaks in the base file cap32.cpp
# There should be 4 Errors, hopefully we don't add any other errors
add_custom_target(valgrind COMMAND cd .. && valgrind --verbose
                                                     --leak-check=full 
                                                     --log-file=${BUILD_DIR}/memcheck-amle-exec.txt 
                                                     ./amle-exec
                           
                           COMMAND cd .. && valgrind --leak-check=full 
                                                     --log-file=${BUILD_DIR}/memcheck-amle-tests.txt 
                                                     ./amle-tests)
                        
# This target runs the amle-exec from the build dir (just a time saver)
add_custom_target(rae COMMAND cd .. && ./amle-exec)

# This target runs the amle-tests from the build dir (just a time saver)
add_custom_target(rat COMMAND cd .. && ./amle-tests)