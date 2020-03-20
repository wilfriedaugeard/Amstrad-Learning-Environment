#include "MemoryUtils.h"

int preProcessValue(int value, int base) {
    if(base == DEC_BASE) {
        return value - value / HEX_MAX_VAL * HEX_NUMBER_LETTERS;
    }
    return value;
}

int convertByteArrayToInt(byte * array, word length, int base) {
    int x = 0;
    int currentExp = 1;

    for(word index = length; index > 0; index--) {
        int val = array[index - 1];
        val = preProcessValue(val, base);
        x += currentExp * val; 
        currentExp *= base;
    }
    return x;
}

byte getShift(StorageType type) {
    if(type == StorageType::ASCII) {
        return ASCII_SHIFT;
    }

    return 0;
}

int getBase(StorageType type) {
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

int readMemoryRange(AddressRange range, StorageType type) {
    word length = (word)range.end - (word)range.start + 1;
    byte container[length];
    byte shift = getShift(type);
    int base = getBase(type);
 
    for(word index = 0; index < length; index++) 
    {
        container[index] = amle_public_read_mem((word)range.start + (word)index) - shift;
    }
    return convertByteArrayToInt(container, length, base);
}

// int readMemoryRangeBis(AddressRange range, bool asciiShift) {
//     word length = (word)range.end - (word)range.start;
//     byte container[range.end - range.start];
 
//     for(word index = 0; index < length; index++) 
//     {
//         container[index] = amle_public_read_mem((word)range.start + (word)index);
//         if(asciiShift) {
//             container[index] = container[index] - ASCII_SHIFT;
//         }
//     }

//     if(asciiShift) {
//         return convertByteArrayToInt(container, length, DEC_BASE);
//     }
//     return convertByteArrayToInt(container, length, BYTE_BASE);
// }

// int readAsciiMemoryRange(AddressRange range) {
//     return readMemoryRange(range, true);
// }