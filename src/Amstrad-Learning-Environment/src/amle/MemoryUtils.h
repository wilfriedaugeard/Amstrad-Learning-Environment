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
 *  MemoryUtils.h
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef MEMORYUTILS_H_
#define MEMORYUTILS_H_

/**
 * \file MemoryUtils.h
 * \brief This files contains functions to read memory addresses.
 * 
 * The Amstrad CPC memory is 65536 (2^16) bytes long. Therefore the values passed to these 
 * functions should only be within the [0, 65535] range. 
 * 
 * To get any crucial information from the memory, the address in memory has to be known before hand.
 * Such addresses can sometimes be found online but otherwise the game has to be disassembled or raw 
 * memory outputs have to be analyzed.
 */ 

#include <iostream>

#include "AddressRange.h"
#include "StorageType.h"
#include "z80.h"

/**
 * This MACRO is used to convert bytes array to integer, it represents a conversion if the value is 
 * ASCII encoded. For more information, read StorageType.
 * 
 * Example : "32 35 38 34" -> "02 05 08 04" -> 4 + 8 * 10 + 5 * 100 + 2 * 1000 -> 2584.
 * Note that an ASCII_SHIFT will be applied.
 */ 
#define ASCII_BASE 10 

/**
 * This MACRO is used to convert bytes array to integer, it represents a conversion if the value is 
 * DECIMAL encoded. For more information, read StorageType.
 * 
 * Example : "10 52" -> 52 + 10 * 100 -> 1052.
 * This conversion is human readable.
 */ 
#define DEC_BASE 100

/**
 * This MACRO is used to convert bytes array to integer, it represents a conversion if the value is 
 * HEXADECIMAL encoded. For more information, read StorageType.
 * 
 * Example : "01 FF" -> 1 * 256 + 15 * 16 + 15 -> 511.
 * Note that this conversion is supposing the number is big endian encoded. Little endian support 
 * might be added later.
 */ 
#define HEX_BASE 256

/**
 * Ascii shift to apply to byte arrays that are ASCII encoded. ASCII encoded values are all in the 
 * range [30, 39], which corresponds to [48, 57] in decimal. Therefore is the shift is applied the 
 * obtained range is [0, 9].  
 */ 
#define ASCII_SHIFT 48

/**
 * This MACRO is used for code readability, it is obvious that the maximum hexadecimal value is 15 (16 is used for strict 
 * inequalities).
 */ 
#define HEX_MAX_VAL 16

/**
 * This MACRO is used for code readability, it is used to state that there are indeed only 6 letters in hexadecimal.
 */ 
#define HEX_NUMBER_LETTERS 6

/**
 * \fn int readMemoryRange(AddressRange range, StorageType type)
 * \brief Reads a continuos range of bytes in the memory and returns the 
 * corresponding integer.
 * \param range: The starting and ending points of the address range. The values stored 
 * within the variable should be valid, if this is not the case, the behaviour is UNDEFINED.
 * \param type: The type of encoding used for the integer that should be read, more on this in StorageType.
 * \return The read integer.
 */ 
int readMemoryRange(AddressRange range, StorageType type);

#endif