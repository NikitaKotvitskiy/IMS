/******************************************************************************
 *                                  MCD
 *                             mcd_kitchen.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of kitchen workers' classes
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

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

// Class of iniciator, who prepare buns
class Iniciator : public KitchenWorker {
    void Behavior();
};

// Class of assambler, who fills buns with garnish
class Assambler : public KitchenWorker {
    void Behavior();
};

// Class of finisher, who does raw fryer and packs burgers and additions
class SingleFinisher : public KitchenWorker {
    void Behavior();
};

// Class of finisher, who does raw fryer only
class FinisherFryer : public KitchenWorker {
    void Behavior();
};

// Class of finisher, who does packing only
class FinisherPacker : public KitchenWorker {
    void Behavior();
};

// Class of griller, who prepare meat
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