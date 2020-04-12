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
 *  MemoryUtils.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "MemoryUtils.h"

int preProcessValue(int value, int base) {

    // Applies shift to values before converting them to actual integers in base 10

    // This conversion has been found empirically
    // There might be some mathematical explanations to why it works
    // but it hasn't been found
    if(base == DEC_BASE) {
        return value - value / HEX_MAX_VAL * HEX_NUMBER_LETTERS;
    }

    // Applying the ASCII shift as explained in the .h file
    if(base == ASCII_BASE) {
        return value - ASCII_SHIFT;
    }

    return value;
}

int getBase(StorageType type) {
    // Switch (actually ifs but it is more readable)
    // Used to determine the current base
    if(type == StorageType::ASCII) {
        return ASCII_BASE;
    }

    if(type == StorageType::DEC) {
        return DEC_BASE;
    }

    if(type == StorageType::HEX) {
        return HEX_BASE;
    }

    return -1;
}

int convertByteArrayToInt(byte * array, word length, int base) {
    // The final value
    int x = 0;

    // The current exposant
    int currentExp = 1;

    // Classic base to base conversion with a processing of the values 
    // depending on their encoding added
    // Note that for the conversion starts from the last index 
    // because so far only big endian encoding is considered
    for(word index = length; index > 0; index--) {
        int val = array[index - 1];
        val = preProcessValue(val, base);
        x += currentExp * val; 
        currentExp *= base;
    }
    return x;
}

int readMemoryRange(AddressRange range, StorageType type) {

    // Number of bytes to read
    word length = (word)range.end - (word)range.start + 1;

    // The containter that will store the raw bytes
    byte container[length];

    // The base the bytes are encoded with
    int base = getBase(type);
 
    // Fetches all the bytes
    for(word index = 0; index < length; index++) 
    {
        container[index] = amle_public_read_mem((word)range.start + (word)index);
    }

    // Returns the decoded integer
    return convertByteArrayToInt(container, length, base);
}