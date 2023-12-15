#ifndef MCD_BEVERAGE
#define MCD_BEVERAGE

#include "simlib.h"

class BeverageWorker : public Process {
public:
    int id;
    void Behavior();
};

class Drink : public Process {
    void Behavior();
};

#endif