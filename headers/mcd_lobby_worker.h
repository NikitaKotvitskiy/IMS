/******************************************************************************
 *                                  MCD
 *                             mcd_lobby_worker.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of lobby worker's classes
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#ifndef MCD_LOBBY
#define MCD_LOBBY

#include "simlib.h"

class LobbyWorker : public Process {
public:
    int id;
    
    void Behavior();
};

#endif