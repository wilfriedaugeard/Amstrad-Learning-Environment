#ifndef CAP32VARIABLES_H_
#define CAP32VARIABLES_H_

#include "cap32Dup.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "SDL/SDL.h"

#include "crtc.h"
#include "disk.h"
#include "tape.h"
#include "video.h"
// #include "z80.h"
#include "configuration.h"
#include "stringutils.h"
#include "zip.h"
#include "keyboard.h"
#include "cartridge.h"
#include "asic.h"
#include "argparse.h"
#include "slotshandler.h"
#include "fileutils.h"

#include <errno.h>
#include <string.h>

#include "CapriceGui.h"
#include "CapriceGuiView.h"
#include "CapriceVKeyboardView.h"

#include "errors.h"
#include "log.h"

#include "savepng.h"

#include "SupportedGamesUtils.h"

#define MAX_LINE_LEN 256

#define MIN_SPEED_SETTING 2
#define MAX_SPEED_SETTING 32
#define DEF_SPEED_SETTING 4

#define MAX_NB_JOYSTICKS 2

#define POLL_INTERVAL_MS 1

extern byte bTapeLevel;
extern t_z80regs z80;

extern dword *ScanPos;
extern dword *ScanStart;
extern word MaxVSync;
extern t_flags1 flags1;
extern t_new_dt new_dt;
extern t_disk_format disk_format[];

extern byte* pbCartridgePages[];

extern SDL_AudioSpec *audio_spec;

extern SDL_Surface *back_surface;
extern video_plugin* vid_plugin;
extern SDL_Joystick* joysticks[MAX_NB_JOYSTICKS];

extern dword dwTicks, dwTicksOffset, dwTicksTarget, dwTicksTargetFPS;
extern dword dwFPS, dwFrameCount;
extern dword dwXScale, dwYScale;
extern dword dwSndBufferCopied;

extern dword osd_timing;
extern std::string osd_message;

extern dword dwBreakPoint, dwTrace, dwMF2ExitAddr;
extern dword dwMF2Flags;
extern byte *pbGPBuffer;
extern byte *pbSndBuffer;
extern byte *pbSndBufferEnd;
extern byte *pbSndStream;
extern byte *membank_read[4], *membank_write[4], *memmap_ROM[256];
extern byte *pbRAM;
extern byte *pbRAMbuffer;
extern byte *pbROM;
extern byte *pbROMlo;
extern byte *pbROMhi;
extern byte *pbExpansionROM;
extern byte *pbMF2ROMbackup;
extern byte *pbMF2ROM;
extern byte *pbTapeImage;
extern byte keyboard_matrix[16];

extern std::list<SDL_Event> virtualKeyboardEvents;
extern dword nextVirtualEventFrameCount, dwFrameCountOverall;
extern dword breakPointsToSkipBeforeProceedingWithVirtualEvents;

extern byte *membank_config[8][4];

extern FILE *pfileObject;
extern FILE *pfoPrinter;

#ifdef DEBUG
dword dwDebugFlag = 0;
FILE *pfoDebug;
#endif

#define MAX_FREQ_ENTRIES 5
extern dword freq_table[MAX_FREQ_ENTRIES];

#include "font.h"

extern double colours_rgb[32][3];

extern double colours_green[32];

extern SDL_Color colours[32];

extern byte bit_values[8];

#include "rom_mods.h"

extern char chAppPath[_MAX_PATH + 1];
extern char chROMSelected[_MAX_PATH + 1];
extern std::string chROMFile[4];

extern t_CPC CPC;
extern t_CRTC CRTC;
extern t_FDC FDC;
extern t_GateArray GateArray;
extern t_PPI PPI;
extern t_PSG PSG;
extern t_VDU VDU;

extern t_drive driveA;
extern t_drive driveB;

#define psg_write \
{ \
   byte control = PSG.control & 0xc0; /* isolate PSG control bits */ \
   if (control == 0xc0) { /* latch address? */ \
      PSG.reg_select = psg_data; /* select new PSG register */ \
   } else if (control == 0x80) { /* write? */ \
      if (PSG.reg_select < 16) { /* valid register? */ \
         SetAYRegister(PSG.reg_select, psg_data); \
      } \
   } \
}

extern CapriceArgs args;

extern std::chrono::time_point<std::chrono::system_clock> inputTimer;

#endif