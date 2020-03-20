#ifndef MEMORYUTILS_H_
#define MEMORTUTILS_H_

#include <iostream>

#include "AddressRange.h"
#include "StorageType.h"
#include "z80.h"

#define ASCII_BASE 10
#define DEC_BASE 100
#define HEX_BASE 256
#define ASCII_SHIFT 48

#define HEX_MAX_VAL 16
#define HEX_NUMBER_LETTERS 6

int convertByteArrayToInt(byte * array);
int readMemoryRangeBis(AddressRange range, bool asciiShift = false);
int readAsciiMemoryRange(AddressRange range);

int readMemoryRange(AddressRange range, StorageType type);

#endif