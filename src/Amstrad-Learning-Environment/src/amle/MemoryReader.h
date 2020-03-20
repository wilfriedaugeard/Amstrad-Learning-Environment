#ifndef MEMORY_READER_HPP_
#define MEMORY_READER_HPP_

#include "z80.h"

class MemoryReader
{
    public:

        /**
         * Default constructor.
         **/
        MemoryReader();

        /**
         * Default destructor.
         **/
        ~MemoryReader();

        /**
         * \fn void printMemoy(word addr)
         * \brief Encapsulates the method amle_public_mem_read that is a copy of the
         * method used by Caprice32 to randomly access the RAM.
         * \param addr: The address of the byte to print.
         */
        void printMemory(word addr);

        /**
         * \fn void printRangeMemory(word start, word end)
         * \brief Allows to print a range of bytes from the RAM.
         * \param start: The address of the first byte.
         * \param end: The address of the last byte.
         **/
        void printRangeMemory(word start, word end);

        /**
         * \fn void getMemoryRange(byte* container, wod start, word length)
         * \brief Gets a chunk of memory of size length and stores the equired bytes in the given container.
         * \param container: The array to store the bytes in.
         * \param start: The address of the first byte to read.
         * \param length: The number of bytes to read.
         **/
        void getMemoryRange(byte* container, word start, word length);

        /**
         * \fn void readScore()
         * \brief Reads the score and stores it in the correct variable.
         **/
        void readScore();

        /**
         * \fn void printScore()
         * \brief Prints the important data fetched.
         **/
        void printScore();

    private:

        // Addesses of important data (to be read from a file later?)
        // Maybe not stored here?
        word scoreAddrStart;
        word scoreSize;
        byte * score;
};

#endif