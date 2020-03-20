/* *****************************************************************************
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.M.L.E (Amstrad Learning Environment)
 * Copyright (c) 2020 by Kévin Durou, Johann Kadionik, Vincent Ameeuw, Wilfried 
 * Augeard and Theo De Castro Pinto.
 * Released under the GNU General Public License; see License.txt for details.
 * 
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Caprice32  --  "An Amstrad CPC Emulator"
 * (c) Copyright 1997-2015 Ulrich Doewich
 * (c) Copyright 2016-2019 Colin Pitrat
 *
 * *****************************************************************************
 *  amle_interface.hpp
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef __AMLE_INTERFACE_HPP__
#define __AMLE_INTERFACE_HPP__

#include <string>
// #include "common/constants.h"

#include "cap32Dup.h"
#include "cap32Variables.h"
#include "SupportedGamesUtils.h"
#include "MemoryUtils.h"

/**
   This class interfaces ALE with external code for controlling agents.
 */
class AmLEInterface {
    public:
        AmLEInterface(int argc, char ** argv);
        ~AmLEInterface();
        void loadROM(SupportedGames game, const char * path);
        int act(SDL_Event *event);
        void reset_game();
        void loadSnapshot(SupportedGames game, const char * path);
        void saveSnapshot(const char * path);
        int getNbLives();
        bool gameOver();
        int getScore();
        std::vector<SDL_Event> getLegalActions();
        long getFrameNumber();
        CPCScreen getRGBScreen();
        void step();

    private:
        int iExitCondition;
        bool bolDone;
        bool take_screenshot;
        SDL_Event event;
        std::vector<std::string> slot_list;
        int previousScore;
        long numberFrame;

      /**

      // Save the current screen as a png file
      void saveScreenPNG(const std::string& filename);

      // Get the value of a setting.
      std::string getString(const std::string& key);
      int getInt(const std::string& key);
      bool getBool(const std::string& key);
      float getFloat(const std::string& key);

      // Set the value of a setting. loadRom() must be called before the
      // setting will take effect.
      void setString(const std::string& key, const std::string& value);
      void setInt(const std::string& key, const int value);
      void setBool(const std::string& key, const bool value);
      void setFloat(const std::string& key, const float value);
    **/

};

#endif  // __AMLE_INTERFACE_HPP__