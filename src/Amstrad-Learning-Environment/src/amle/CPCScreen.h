#ifndef CPCSCREEN_H_
#define CPCSCREEN_H_

typedef struct {
    int width;
    int height;
    std::vector<unsigned int> pixels; 
}CPCScreen;

#endif