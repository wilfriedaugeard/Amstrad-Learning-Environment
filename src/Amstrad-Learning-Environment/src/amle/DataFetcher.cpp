#include "DataFetcher.h"

DataFetcher::DataFetcher()
{

}

DataFetcher::~DataFetcher()
{

}

void DataFetcher::main(MemoryReader * memoryReader)
{
    memoryReader->readScore();
}