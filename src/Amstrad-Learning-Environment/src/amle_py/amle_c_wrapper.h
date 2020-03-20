#ifndef __AMLE_C_WRAPPER_H__
#define __AMLE_C_WRAPPER_H__

#include <amle_interface.hpp>

extern "C" {

    amle::AmLEInterface *AmLE_new(int argc, char** argv) {return new amle::AmLEInterface(argc, argv);}
    
    void AmLE_del(ale::AmLEInterface *amle){delete amle;}

    void loadRom(ale::AmLEInterface *amle, SupportedGames game, const char * path){amle->loadRom(game, path);}

}

#endif