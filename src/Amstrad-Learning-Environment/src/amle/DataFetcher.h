#ifndef DATA_FETCHER_H_
#define DATA_FETCHER_H_

#include "MemoryReader.h"

class DataFetcher
{
    public:

        /**
         * Default constructor.
         **/
        DataFetcher();

        /**
         * Default destructor
         **/
        ~DataFetcher();

        /**
         * \fn main(MemoryReader* memoryReader)
         * \brief Main method of the DataFetcher.
         **/
        void main(MemoryReader * memoryReader);
};

#endif