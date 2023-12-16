/******************************************************************************
 *                                  MCD
 *                          mcd_service_worker.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of beverage worker class
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

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