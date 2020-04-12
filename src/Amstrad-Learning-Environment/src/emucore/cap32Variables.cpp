#include "cap32Variables.h"

// extern byte bTapeLevel;
// extern t_z80regs z80;

// extern dword *ScanPos;
// extern dword *ScanStart;
// extern word MaxVSync;
// extern t_flags1 flags1;
// extern t_new_dt new_dt;
// extern t_disk_format disk_format[];

// extern byte* pbCartridgePages[];

SDL_AudioSpec *audio_spec = nullptr;

SDL_Surface *back_surface = nullptr;
video_plugin* vid_plugin;
SDL_Joystick* joysticks[MAX_NB_JOYSTICKS];

dword dwTicks, dwTicksOffset, dwTicksTarget, dwTicksTargetFPS;
dword dwFPS, dwFrameCount;
dword dwXScale, dwYScale;
dword dwSndBufferCopied;

dword osd_timing;
std::string osd_message;

dword dwBreakPoint, dwTrace, dwMF2ExitAddr;
dword dwMF2Flags = 0;
byte *pbGPBuffer = nullptr;
byte *pbSndBuffer = nullptr;
byte *pbSndBufferEnd = nullptr;
byte *pbSndStream = nullptr;
byte *membank_read[4], *membank_write[4], *memmap_ROM[256];
byte *pbRAM = nullptr;
byte *pbRAMbuffer = nullptr;
byte *pbROM = nullptr;
byte *pbROMlo = nullptr;
byte *pbROMhi = nullptr;
byte *pbExpansionROM = nullptr;
byte *pbMF2ROMbackup = nullptr;
byte *pbMF2ROM = nullptr;
byte *pbTapeImage = nullptr;
byte keyboard_matrix[16];

std::list<SDL_Event> virtualKeyboardEvents;
dword nextVirtualEventFrameCount, dwFrameCountOverall = 0;
dword breakPointsToSkipBeforeProceedingWithVirtualEvents = 0;

byte *membank_config[8][4];

FILE *pfileObject;
FILE *pfoPrinter;

#ifdef DEBUG
dword dwDebugFlag = 0;
FILE *pfoDebug = nullptr;
#endif

#define MAX_FREQ_ENTRIES 5
dword freq_table[MAX_FREQ_ENTRIES] = {
   11025,
   22050,
   44100,
   48000,
   96000
};

double colours_rgb[32][3] = {
   { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 },{ 0.0, 1.0, 0.5 }, { 1.0, 1.0, 0.5 },
   { 0.0, 0.0, 0.5 }, { 1.0, 0.0, 0.5 },{ 0.0, 0.5, 0.5 }, { 1.0, 0.5, 0.5 },
   { 1.0, 0.0, 0.5 }, { 1.0, 1.0, 0.5 },{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 },
   { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 },{ 1.0, 0.5, 0.0 }, { 1.0, 0.5, 1.0 },
   { 0.0, 0.0, 0.5 }, { 0.0, 1.0, 0.5 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0 },
   { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },{ 0.0, 0.5, 0.0 }, { 0.0, 0.5, 1.0 },
   { 0.5, 0.0, 0.5 }, { 0.5, 1.0, 0.5 },{ 0.5, 1.0, 0.0 }, { 0.5, 1.0, 1.0 },
   { 0.5, 0.0, 0.0 }, { 0.5, 0.0, 1.0 },{ 0.5, 0.5, 0.0 }, { 0.5, 0.5, 1.0 }
};

double colours_green[32] = {
   0.5647, 0.5647, 0.7529, 0.9412,
   0.1882, 0.3765, 0.4706, 0.6588,
   0.3765, 0.9412, 0.9098, 0.9725,
   0.3451, 0.4078, 0.6275, 0.6902,
   0.1882, 0.7529, 0.7216, 0.7843,
   0.1569, 0.2196, 0.4392, 0.5020,
   0.2824, 0.8471, 0.8157, 0.8784,
   0.2510, 0.3137, 0.5333, 0.5961
};

SDL_Color colours[32];

byte bit_values[8] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

#define MAX_ROM_MODS 2

char chAppPath[_MAX_PATH + 1];
char chROMSelected[_MAX_PATH + 1];
std::string chROMFile[4] = {
   "cpc464.rom",
   "cpc664.rom",
   "cpc6128.rom",
   "system.cpr"
};

t_CPC CPC;
t_CRTC CRTC;
t_FDC FDC;
t_GateArray GateArray;
t_PPI PPI;
t_PSG PSG;
t_VDU VDU;

t_drive driveA;
t_drive driveB;

CapriceArgs args;

std::chrono::time_point<std::chrono::system_clock> inputTimer = std::chrono::system_clock::now();

GameInfo currentGame;