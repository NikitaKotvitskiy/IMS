#ifndef MCD_KITCHEN
#define MCD_KITCHEN

#include "simlib.h"

class KitchenWorker : public Process {
public:
    int id;
    
    void transferRaw();
    void prepareRaw();
    void packAnAddition();
    void packABurger();
    void fillABurger();
    void transferBuns();
    void prepareBuns();
};

class Iniciator : public KitchenWorker {
    void Behavior();
};

class Assambler : public KitchenWorker {
    void Behavior();
};

class SingleFinisher : public KitchenWorker {
    void Behavior();
};

class FinisherFryer : public KitchenWorker {
    void Behavior();
};

class FinisherPacker : public KitchenWorker {
    void Behavior();
};

class Griller : public Process {
    void Behavior();
};

class Raw : public Process {
    void Behavior();
};

class Bun : public Process {
    void Behavior();
};

class Meat : public Process {
    void Behavior();
};

#endif