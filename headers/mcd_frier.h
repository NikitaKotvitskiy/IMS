/******************************************************************************
 *                                  MCD
 *                             mcd_frier.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of fries fryer class
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

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