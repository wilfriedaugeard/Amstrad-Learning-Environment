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
 *  StorageType.h
 *
 *  The shared library interface.
 **************************************************************************** */

#ifndef StorageType_H_
#define StorageType_H_

/** \file StorageType.h
 * \brief StorageType is an enumeration representing the different possible encodings possible in the memory, especially 
 * regarding the integers. For instance, some game do NOT store all their integers as hexadecimal values corresponding to the 
 * actual decimal expected value.
 * 
 * As a concrete example, in the memory of Arkanoid may be found the current score : "32 35 38 34". This could be 
 * interpreted as being a little endian encoding : "876098866". But this would be incorrect. This is actually 
 * an ASCII encoding (probably used because it is then easy to display). So the correct way to convert it, is to 
 * get the corresponding ASCII character of each byte : "2584".
 * 
 * So far, there are two types of encoding found in the memory of games and one is expected to exist :
 * 
 * - ASCII : "32 35 38 34" = "2584". Get the ASCII value of each byte. (Ex: Arkanoid)
 * 
 * - HEX : "32 35 38 34" = "842348596". Big Endian encoding. (Never found yet)
 * 
 * - DEC : "32 35 38 34" = "32353834". Human readable encoding. (Ex: Buggy Boy)
 */ 
enum StorageType {
    ASCII, /**< ASCII encoding, see detailled description for more info. */
    HEX, /**< HEX encoding, see detailled description for more info. */
    DEC /**< DEC encoding, see detailled description for more info. */
};

#endif