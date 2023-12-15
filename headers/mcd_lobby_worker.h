#ifndef MCD_LOBBY
#define MCD_LOBBY

#include "simlib.h"

class LobbyWorker : public Process {
public:
    int id;
    
    void Behavior();
};

#endif