#ifndef MCD_FRIER
#define MCD_FRIER

#include "simlib.h"

class Frier : public Process {
    void Behavior();
};

class FryingFries : public Process {
    void Behavior();
};

#endif