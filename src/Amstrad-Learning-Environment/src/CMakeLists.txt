cmake_minimum_required (VERSION 2.6)
project(amle)
set(AMLEVERSION "0.1.0")

# Some relative locations (self explanatory)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_DIR ${SOURCE_DIR}/test)
set(BUILD_DIR ${SOURCE_DIR}/build)
set(HOME $ENV{HOME})

# set(UNIX 0)
# set(WIN32 1)
# set(WIN64 1)

# Compiler for UNIX
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUNIT_CFLAGS} -std=c++11 -fPIC -g -fomit-frame-pointer -D__STDC_CONSTANT_MACROS")

  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused")
  endif()
  SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name")
endif()

# Compiler for windows
if(WIN32)
    set(CMAKE_SYSTEM_NAME Windows)

    add_definitions(-DWINDOWS=1)

    SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
    SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
    SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
    set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ipath/usr/i686-w64-mingw32/include -Lpath/usr/i686-w64-mingw32/lib")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    set(WIN_LIBS mingw32 SDLmain SDL)

elseif(WIN64)
    set(CMAKE_SYSTEM_NAME Windows)

    add_definitions(-DWINDOWS=1)

    SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
    set(CMAKE_RANLIB x86_64-w64-mingw32-ranlib)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ipath/usr/x86_64-w64-mingw32/include -Lpath/usr/x86_64-w64-mingw32/lib")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    set(WIN_LIBS mingw32 SDLmain SDL)
endif()

# message(${LIB_INSTALL_DIR})

# The list of directories containing c/cpp files
##############################################################################
# IMPORTANT : If you create a new directory, please make sure to add it here !
##############################################################################

set(MODULES common controllers emucore environment games external external amle)
set(TEST_MODULES test)

# Finds all the c/cpp files from the different directories added above and ads them all a
# variable SOURCES. Used below when adding a new executable
foreach(module ${MODULES})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c)
  list(APPEND SOURCES ${module_sources})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c?[xp])
  list(APPEND SOURCES ${module_sources})
endforeach(module ${MODULES})

foreach(module ${TEST_MODULES})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c)
  list(APPEND TEST_SOURCES ${module_sources})
  file(GLOB module_sources ${SOURCE_DIR}/${module}/*.c?[xp])
  list(APPEND TEST_SOURCES ${module_sources})
endforeach(module ${TEST_MODULES})

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
  # ${SOURCE_DIR}/test
  /usr/include/freetype2
  usr/include/cppunit
)

# Main (and only) library of the project, we compile it here so we don't have to compile
# it for each executable
add_library(libamle ${SOURCES})

if(UNIX)
  # Create the main executable from the project (although we will hopefully only
  # be dispatching a library, this is useful to check everything is working)
  add_executable(amle-bin ${SOURCE_DIR}/main.cpp)
  set_target_properties(amle-bin PROPERTIES OUTPUT_NAME amle-exec)
  set_target_properties(amle-bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(amle-bin libamle ${LINK_LIBS} z png freetype)

  # Create an executable that runs tests which will surely make sure our project is flawless
  add_executable(amle-tests-bin ${TEST_DIR}/mainTest.cpp ${TEST_SOURCES})
  set_target_properties(amle-tests-bin PROPERTIES OUTPUT_NAME amle-tests)
  set_target_properties(amle-tests-bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(amle-tests-bin libamle ${LINK_LIBS} z png freetype cppunit)

endif()

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
add_custom_target(rat COMMAND cd .. && ./amle-tests
                      COMMAND cd ../test/ && python3 testPython.py)

# This targets runs the test python file
add_custom_target(rpf COMMAND cd .. && python3 test.py)

# Builds package amle-py ready for install
add_custom_target(build-amle-py COMMAND cd .. &&
                                        rm -rf build/lib/ *.egg-info &&
                                        python3 setup.py sdist bdist_wheel)

# Install the lib on the computer
add_custom_target(install-amle-py COMMAND cd .. && cd dist && pip3 install *.whl)

# Special command to build the library for windows
if(WIN32)
  add_custom_target(build-amle_c-32 COMMAND ${CMAKE_CXX_COMPILER}
                        -shared -o amle_c-32.dll
                        CMakeFiles/libamle.dir/amle/*.o
                        CMakeFiles/amle-c-lib.dir/amle_py/*.o
                        CMakeFiles/libamle.dir/emucore/*.o
                        -static-libgcc -static-libstdc++
                        -lmingw32 -lSDLmain -lSDL -lpng -lz -lfreetype &&
                        mv amle_c-32.dll ../amle_py/DLL/32Bit)
elseif(WIN64)
  add_custom_target(build-amle_c-64 COMMAND ${CMAKE_CXX_COMPILER}
                        -shared -o amle_c-64.dll
                        CMakeFiles/libamle.dir/amle/*.o
                        CMakeFiles/amle-c-lib.dir/amle_py/*.o
                        CMakeFiles/libamle.dir/emucore/*.o
                        -static-libgcc -static-libstdc++
                        -lmingw32 -lSDLmain -lSDL -lpng -lz -lfreetype &&
                        mv amle_c-64.dll ../amle_py/DLL/64Bit)
endif()

add_library(amle-c-lib SHARED ${CMAKE_CURRENT_SOURCE_DIR}/amle_py/amle_c_wrapper.cpp ${SOURCE_DIR}/amle/amle_interface.cpp)
set_target_properties(amle-c-lib PROPERTIES OUTPUT_NAME amle_c)
set_target_properties(amle-c-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/amle_py)
if(PYTHON_MODULE_EXTENSION)
    set_target_properties(amle-c-lib PROPERTIES SUFFIX "${PYTHON_MODULE_EXTENSION}")
endif()

if(WIN32 OR WIN64)
    target_link_libraries(amle-c-lib libamle ${WIN_LIBS} png z freetype)
endif()

if(UNIX)
    install(TARGETS amle-c-lib DESTINATION ${LIB_INSTALL_DIR})
endif()

if(UNIX)
    target_link_libraries(amle-c-lib libamle ${LINK_LIBS} png z freetype)
endif()
