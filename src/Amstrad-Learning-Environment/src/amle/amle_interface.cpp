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
 *  amle_interface.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "amle_interface.h"

enum ApplicationWindowState
{
   Minimized,              // application window has been iconified
   Restored,               // application window has been restored
   GainedFocus,            // application window got input focus
   LostFocus               // application window lost input focus
} _appWindowState;

AmLEInterface::AmLEInterface(int argc, char ** argv) {
    take_screenshot = false;

    parseArguments(argc, argv, slot_list, args);

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0) { // initialize SDL
        fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        exit(-1);
    }

    #ifndef APP_PATH
    if(getcwd(chAppPath, sizeof(chAppPath)-1) == nullptr) {
        fprintf(stderr, "getcwd failed: %s\n", strerror(errno));
        cleanExit(-1);
    }
    #else
        strncpy(chAppPath,APP_PATH,_MAX_PATH);
    #endif

    loadConfiguration(CPC, getConfigurationFilename()); // retrieve the emulator configuration
    if (CPC.printer) {
        if (!printer_start()) { // start capturing printer output, if enabled
            CPC.printer = 0;
        }
    }

    z80_init_tables(); // init Z80 emulation

    if (video_init()) {
        fprintf(stderr, "video_init() failed. Aborting.\n");
        cleanExit(-1);
    }

    if (audio_init()) {
        fprintf(stderr, "audio_init() failed. Disabling sound.\n");
        // TODO(cpitrat): Do not set this to 0 when audio_init fail as this affect
        // configuration when saving from GUI. Rather use some other indicator to
        // know whether snd_bufferptr is usable or not.
        // To test it, set SDL_AUDIODRIVER=dsp or some other unsupported value.
        CPC.snd_enabled = 0; // disable sound emulation
    }

    if (joysticks_init()) {
        fprintf(stderr, "joysticks_init() failed. Joysticks won't work.\n");
    }

    #ifdef DEBUG
    pfoDebug = fopen("./debug.txt", "wt");
    #endif

    // Extract files to be loaded from the command line args
    fillSlots(slot_list, CPC);

    // Must be done before emulator_init()
    CPC.InputMapper = new InputMapper(&CPC);

    // emulator_init must be called before loading files as they require
    // pbGPBuffer to be initialized.
    if (emulator_init()) {
        fprintf(stderr, "emulator_init() failed. Aborting.\n");
        cleanExit(-1);
    }

    // Really load the various drives, if needed
    loadSlots();

    // Fill the buffer with autocmd if provided
    virtualKeyboardEvents = CPC.InputMapper->StringToEvents(args.autocmd);
    // Give some time to the CPC to start before sending any command
    nextVirtualEventFrameCount = dwFrameCountOverall + CPC.boot_time;

    // ----------------------------------------------------------------------------

    update_timings();
    audio_resume();

    iExitCondition = EC_FRAME_COMPLETE;
    bolDone = false;

    previousScore = 0;
    numberFrame = 0;
}

AmLEInterface::~AmLEInterface() {

}

/*
void AmLEInterface::loadROM(const char * path) {
    file_load(path, DSK_A);
    virtualKeyboardEvents = CPC.InputMapper->StringToEvents(" run\"arkanoid\n");
}
*/

void fillLegalActions() {
    std::vector<char> tmp = currentGame.getLegalActionsAsChars();

    for(unsigned int i = 0; i < tmp.size(); i++) {
        unsigned int uintAction = CPC.InputMapper->uintFromChar(tmp[i]);
        SDL_Event event = uintToSDLEvent(uintAction);
        event.type = SDL_KEYDOWN;
        currentGame.addLegalActionAsEvent(event);
        event.type = SDL_KEYUP;
        currentGame.addLegalActionAsEvent(event);
    }
}

void loadGameData(SupportedGames game) {
    currentGame = getGameFromEnum(game);
    std::string runcmd(" run\"");
    runcmd += currentGame.getName();
    runcmd += "\n";
    virtualKeyboardEvents = CPC.InputMapper->StringToEvents(runcmd.c_str());
    fillLegalActions();
}

void AmLEInterface::loadROM(SupportedGames game, const char * path) {
    file_load(path, DSK_A);
    loadGameData(game);
}

int AmLEInterface::act(SDL_Event *event) {
    SDL_PushEvent(event);
    this->step();
    int deltaScore = this->getScore() - this->previousScore;
    this->previousScore = this->getScore();
    return deltaScore;
}

void AmLEInterface::loadSnapshot(SupportedGames game, const char * path) {
    snapshot_load(path);
    loadGameData(game);
}

void AmLEInterface::saveSnapshot(const char * path) {
    snapshot_save(path);
}

void AmLEInterface::reset_game() {
    emulator_reset(false);
}

int AmLEInterface::getNbLives() {
    return amle_public_read_mem(currentGame.getLivesAddress());
}

bool AmLEInterface::gameOver() {
    return currentGame.getGameOverExpectedValue() == 
            readMemoryRange(currentGame.getGameOverAddressRange(), currentGame.getGameOverValueType());
}

int AmLEInterface::getScore() {
    return readMemoryRange(currentGame.getScoreAddressRange(), currentGame.getScoreType());
}

std::vector<SDL_Event> AmLEInterface::getLegalActions() {
    return currentGame.getLegalActionsAsEvents();
}

long AmLEInterface::getFrameNumber() {
    return this->numberFrame;
}

CPCScreen AmLEInterface::getRGBScreen() {
    return getScreen();
}

void AmLEInterface::step() {
    this->numberFrame++;
    if(!virtualKeyboardEvents.empty()
        && (nextVirtualEventFrameCount < dwFrameCountOverall)
        && (breakPointsToSkipBeforeProceedingWithVirtualEvents == 0)) {

        auto nextVirtualEvent = &virtualKeyboardEvents.front();
        SDL_PushEvent(nextVirtualEvent);
    
        auto keysym = nextVirtualEvent->key.keysym;
        LOG_DEBUG("Inserted virtual event keysym=" << int(keysym.sym));
    
        dword cpc_key = CPC.InputMapper->CPCkeyFromKeysym(keysym);
        if (!(cpc_key & MOD_EMU_KEY)) {
        LOG_DEBUG("The virtual event is a keypress (not a command), so introduce a pause.");
        // Setting nextVirtualEventFrameCount below guarantees to
        // immediately break the loop enclosing this code and wait
        // at least one frame.
        nextVirtualEventFrameCount = dwFrameCountOverall
            + ((event.type == SDL_KEYDOWN)?1:0);
        // The extra delay in case of SDL_KEYDOWN is to keep the
        // key pressed long enough.  If we don't do this, the CPC
        // firmware debouncer eats repeated characters.
        }

        virtualKeyboardEvents.pop_front();
    }
    

    while (SDL_PollEvent(&event)) {
    
        switch (event.type) {
        case SDL_KEYDOWN:
            {
                dword cpc_key = CPC.InputMapper->CPCkeyFromKeysym(event.key.keysym);
                if (!(cpc_key & MOD_EMU_KEY)) {
                    applyKeypress(cpc_key, keyboard_matrix, true);
                }
            }
            break;

        case SDL_KEYUP:
            {
                dword cpc_key = CPC.InputMapper->CPCkeyFromKeysym(event.key.keysym);
                if (!(cpc_key & MOD_EMU_KEY)) {
                    applyKeypress(cpc_key, keyboard_matrix, false);
                }
                else { // process emulator specific keys
                    switch (cpc_key) {

                    case CAP32_GUI:
                        {
                        showGui();
                        break;
                        }

                    case CAP32_VKBD:
                        {
                        showVKeyboard();
                        break;
                        }

                    case CAP32_FULLSCRN:
                        audio_pause();
                        SDL_Delay(20);
                        video_shutdown();
                        CPC.scr_window = CPC.scr_window ? 0 : 1;
                        if (video_init()) {
                            fprintf(stderr, "video_init() failed. Aborting.\n");
                            cleanExit(-1);
                        }
                        audio_resume();
                        break;

                    case CAP32_SCRNSHOT:
                        // Delay taking the screenshot to ensure the frame is complete.
                        take_screenshot = true;
                        break;

                    case CAP32_DELAY:
                        // Reuse boot_time as it is a reasonable wait time for Plus transition between the F1/F2 nag screen and the command line.
                        // TODO: Support an argument to CAP32_DELAY in autocmd instead.
                        LOG_VERBOSE("Take into account CAP32_DELAY");
                        nextVirtualEventFrameCount = dwFrameCountOverall + CPC.boot_time;
                        break;

                    case CAP32_WAITBREAK:
                        breakPointsToSkipBeforeProceedingWithVirtualEvents++;
                        LOG_INFO("Will skip " << breakPointsToSkipBeforeProceedingWithVirtualEvents << " before processing more virtual events.");
                        LOG_DEBUG("Setting z80.break_point=0 (was " << z80.break_point << ").");
                        z80.break_point = 0; // set break point to address 0. FIXME would be interesting to change this via a parameter of CAP32_WAITBREAK on command line.
                        break;

                    case CAP32_SNAPSHOT:
                        dumpSnapshot();
                        break;

                    case CAP32_TAPEPLAY:
                        LOG_DEBUG("Request to play tape");
                        Tape_Rewind();
                        if (pbTapeImage) {
                            if (CPC.tape_play_button) {
                                LOG_DEBUG("Play button released");
                                CPC.tape_play_button = 0;
                            } else {
                                LOG_DEBUG("Play button pushed");
                                CPC.tape_play_button = 0x10;
                            }
                        }
                        set_osd_message(std::string("Play tape: ") + (CPC.tape_play_button ? "on" : "off"));
                        break;

                    case CAP32_MF2STOP:
                        if(CPC.mf2 && !(dwMF2Flags & MF2_ACTIVE)) {
                            reg_pair port;

                            // Set mode to activate ROM_config
                            //port.b.h = 0x40;
                            //z80_OUT_handler(port, 128);

                            // Attempt to load MF2 in lower ROM (can fail if lower ROM is not active)
                            port.b.h = 0xfe;
                            port.b.l = 0xe8;
                            dwMF2Flags &= ~MF2_INVISIBLE;
                            z80_OUT_handler(port, 0);

                            // Stop execution if load succeeded
                            if(dwMF2Flags & MF2_ACTIVE) {
                            z80_mf2stop();
                            }
                        }
                        break;

                    case CAP32_RESET:
                        LOG_DEBUG("User requested emulator reset");
                        emulator_reset(false);
                        break;

                    case CAP32_JOY:
                        CPC.joystick_emulation = CPC.joystick_emulation ? 0 : 1;
                        CPC.InputMapper->set_joystick_emulation();
                        set_osd_message(std::string("Joystick emulation: ") + (CPC.joystick_emulation ? "on" : "off"));
                        break;

                    case CAP32_EXIT:
                        cleanExit (0);
                        break;

                    case CAP32_FPS:
                        CPC.scr_fps = CPC.scr_fps ? 0 : 1; // toggle fps display on or off
                        set_osd_message(std::string("Performances info: ") + (CPC.scr_fps ? "on" : "off"));
                        break;

                    case CAP32_SPEED:
                        CPC.limit_speed = CPC.limit_speed ? 0 : 1;
                        set_osd_message(std::string("Limit speed: ") + (CPC.limit_speed ? "on" : "off"));
                        break;

                    case CAP32_DEBUG:
                        log_verbose = !log_verbose;
                        #ifdef DEBUG
                        dwDebugFlag = dwDebugFlag ? 0 : 1;
                        #endif
                        #ifdef DEBUG_CRTC
                        if (dwDebugFlag) {
                            for (int n = 0; n < 14; n++) {
                                fprintf(pfoDebug, "%02x = %02x\r\n", n, CRTC.registers[n]);
                            }
                        }
                        #endif
                        set_osd_message(std::string("Debug mode: ") + (log_verbose ? "on" : "off"));
                        break;
                    }
                }
            }
            break;

        case SDL_JOYBUTTONDOWN:
        {
            dword cpc_key = CPC.InputMapper->CPCkeyFromJoystickButton(event.jbutton);
                            if (cpc_key == 0xff) {
                if (event.jbutton.button == CPC.joystick_menu_button)
                {
                showGui();
                }
                if (event.jbutton.button == CPC.joystick_vkeyboard_button)
                {
                showVKeyboard();
                }
            }
            applyKeypress(cpc_key, keyboard_matrix, true);
        }
        break;

        case SDL_JOYBUTTONUP:
        {
            dword cpc_key = CPC.InputMapper->CPCkeyFromJoystickButton(event.jbutton);
            applyKeypress(cpc_key, keyboard_matrix, false);
        }
        break;

        case SDL_JOYAXISMOTION:
        {
            dword cpc_key[2] = {0xff, 0xff};
            bool release = false;
            CPC.InputMapper->CPCkeyFromJoystickAxis(event.jaxis, cpc_key, release);
            applyKeypress(cpc_key[0], keyboard_matrix, !release);
            if (release && cpc_key[0] != 0xff) {
                applyKeypress(cpc_key[1], keyboard_matrix, !release);
            }
        }
        break;

        // Code shamelessly copied from http://sdl.beuc.net/sdl.wiki/Event_Examples
        // TODO: What if we were paused because of other reason than losing focus and then only lost focus
        //       the right thing to do here is to restore focus but keep paused... implementing this require
        //       keeping track of pause source, which will be a pain.
        case SDL_ACTIVEEVENT:
            if (event.active.state == (SDL_APPINPUTFOCUS | SDL_APPACTIVE) ) {
                if (event.active.gain == 0) {
                    _appWindowState = Minimized;
                    cpc_pause(); // Always paused when iconified
                } else {
                    if (_appWindowState == LostFocus ) {
                        _appWindowState = GainedFocus;
                        if (CPC.auto_pause)
                        cpc_resume();
                    } else {
                        _appWindowState = Restored;
                        cpc_resume(); // Always unpause when restoring from iconified
                    }
                }
            }
            else if (event.active.state & SDL_APPINPUTFOCUS) {
                if (event.active.gain == 0) {
                    _appWindowState = LostFocus;
                    if (CPC.auto_pause)
                        cpc_pause();
                }
                else {
                    _appWindowState = GainedFocus;
                    if (CPC.auto_pause)
                        cpc_resume();
                }
            }
            break;

        case SDL_QUIT:
            cleanExit(0);
        }
    }

    if (!CPC.paused) { // run the emulation, as long as the user doesn't pause it
        dwTicks = SDL_GetTicks();
        if (dwTicks >= dwTicksTargetFPS) { // update FPS counter?
        dwFPS = dwFrameCount;
        dwFrameCount = 0;
        dwTicksTargetFPS = dwTicks + 1000; // prep counter for the next run
        }

        if (CPC.limit_speed) { // limit to original CPC speed?
        if (CPC.snd_enabled) {
            if (iExitCondition == EC_SOUND_BUFFER) { // Emulation filled a sound buffer.
                if (!dwSndBufferCopied) {
                    return; // delay emulation until our audio callback copied and played the buffer
                }
                dwSndBufferCopied = 0;
            }
        } else if (iExitCondition == EC_CYCLE_COUNT) {
            dwTicks = SDL_GetTicks();
            if (dwTicks < dwTicksTarget) { // limit speed ?
                if (dwTicksTarget - dwTicks > POLL_INTERVAL_MS) { // No need to burn cycles if next event is far away
                    std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
                }
                return; // delay emulation
            }
            dwTicksTarget = dwTicks + dwTicksOffset; // prep counter for the next run
        }
        }

        if (!vid_plugin->lock()) { // lock the video buffer
            return; // skip the emulation if we can't get a lock
        }
        dword dwOffset = CPC.scr_pos - CPC.scr_base; // offset in current surface row
        if (VDU.scrln > 0) {
        CPC.scr_base = static_cast<byte *>(back_surface->pixels) + (VDU.scrln * CPC.scr_line_offs); // determine current position
        } else {
        CPC.scr_base = static_cast<byte *>(back_surface->pixels); // reset to surface start
        }
        CPC.scr_pos = CPC.scr_base + dwOffset; // update current rendering position

        iExitCondition = z80_execute(); // run the emulation until an exit condition is met
    
        if (iExitCondition == EC_BREAKPOINT) {
        // We have to clear breakpoint to let the z80 emulator move on.
        z80.break_point = 0xffffffff; // clear break point
        z80.trace = 1; // make sure we'll be here to rearm break point at the next z80 instruction.

        if (breakPointsToSkipBeforeProceedingWithVirtualEvents>0) {
            breakPointsToSkipBeforeProceedingWithVirtualEvents--;
            LOG_DEBUG("Decremented breakpoint skip counter to " << breakPointsToSkipBeforeProceedingWithVirtualEvents);
        }
        } else {
        if (z80.break_point == 0xffffffff) { // TODO(cpcitor) clean up 0xffffffff into a value like Z80_BREAKPOINT_NONE
            LOG_DEBUG("Rearming EC_BREAKPOINT.");
            z80.break_point = 0; // set break point for next time
        }
        }

        if (iExitCondition == EC_FRAME_COMPLETE) { // emulation finished rendering a complete frame?
        dwFrameCountOverall++;
        dwFrameCount++;
        if (SDL_GetTicks() < osd_timing) {
            print(static_cast<byte *>(back_surface->pixels) + CPC.scr_line_offs, osd_message.c_str(), true);
        } else if (CPC.scr_fps) {
            char chStr[15];
            sprintf(chStr, "%3dFPS %3d%%", static_cast<int>(dwFPS), static_cast<int>(dwFPS) * 100 / (1000 / static_cast<int>(FRAME_PERIOD_MS)));
            print(static_cast<byte *>(back_surface->pixels) + CPC.scr_line_offs, chStr, true); // display the frames per second counter
        }
        asic_draw_sprites();
        vid_plugin->unlock();
        video_display(); // update PC display
        if (take_screenshot) {
            dumpScreen();
            take_screenshot = false;
        }
        } else {
        vid_plugin->unlock();
        }
    }
    else { // We are paused. No need to burn CPU cycles
        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
    }
}