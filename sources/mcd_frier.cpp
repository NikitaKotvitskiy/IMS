/******************************************************************************
 *                                  MCD
 *                              mcd_frier.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Definition of fries frier workers' behavior
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

int friesReady = 0;
int friesPreparing = 0;
int friesFinished = 0;

void Frier::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        // If there are ready fries on the fryer, worker will transfer them to the tray
        if (friesFinished != 0) {
            if (FRIES_DEBUG_MODE) cout << Time << ": fries worker is transfering ready fries to tray" << endl;
            friesFinished--;
            Wait(Normal(friesTransferTime.center, friesTransferTime.scattering));
            friesReady += friesPortionsInSlot;
            continue;
        }

        // If worker realizes, that the count of ready, prepating and needed fries is less than the count he tries to follow, he will prepare new fries
        if (friesReady - friesOrderCount + friesPreparing * friesPortionsInSlot < minimalFries && friesPreparing < frierSlotsCount) {
            if (FRIES_DEBUG_MODE) cout << Time << ": fries worker is preparing new fries" << endl;
            Wait(Normal(friesFryingPrepareTime.center, friesFryingPrepareTime.scattering));
            (new FryingFries)->Activate(Time);
            friesPreparing++;
            continue;
        }

        // If there is fries order, worker will pack a portion of fries
        if (friesOrderCount != 0) {
            if (FRIES_DEBUG_MODE) cout << Time << ": fries worker is packing fries" << endl;
            friesOrderCount--;
            friesReady--;
            Wait(Normal(friesPackingTime.center, friesPackingTime.scattering));
            friesPacked++;

            friesTime(Time - friesOrderTimes.front());
            wholeFriesTime(Time - friesOrderTimes.front());
            friesOrderTimes.pop();
        }
    }
}

void FryingFries::Behavior() {
    Wait(friesPreparingTime);
    friesPreparing--;
    friesFinished++;
}