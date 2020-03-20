#include "MemoryReader.h"

#include "stdio.h"

MemoryReader::MemoryReader()
{
    this->scoreAddrStart = 6;
    this->scoreSize = 6;

    this->score = (byte *)malloc(this->scoreSize * sizeof(byte));
    for(unsigned int i = 0; i < this->scoreSize; i++)
    {
        this->score[i] = 0;
    }
}

MemoryReader::~MemoryReader()
{
    free(this->score);
}

void MemoryReader::printMemory(word addr)
{
    printf("%c", amle_public_read_mem(addr));
}

void MemoryReader::printRangeMemory(word start, word end)
{
    for(word i = start; i <= end; i++) 
    {
        printMemory(i);
    }
}

void MemoryReader::getMemoryRange(byte* container, word start, word length) 
{
    if(container) 
    {
        for(word index = 0; index < length; index++) 
        {
            container[index] = amle_public_read_mem(start + (word)index);
        }
    }
}

void MemoryReader::readScore()
{
    this->getMemoryRange(this->score, this->scoreAddrStart, this->scoreSize);
}

void MemoryReader::printScore()
{
    for(word i = 0; i < this->scoreSize; i++)
    {
        printf("%c", this->score[i]);
    }
}