#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

int friesReady = 0;
int friesPreparing = 0;
int friesFinished = 0;

void Frier::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (friesFinished != 0) {
            if (FRIES_DEBUG_MODE) cout << Time << ": fries worker is transfering ready fries to tray" << endl;
            friesFinished--;
            Wait(Normal(friesTransferTime.center, friesTransferTime.scattering));
            friesReady += friesPortionsInSlot;
            continue;
        }

        if (friesReady - friesOrderCount + friesPreparing * friesPortionsInSlot < minimalFries && friesPreparing < frierSlotsCount) {
            if (FRIES_DEBUG_MODE) cout << Time << ": fries worker is preparing new fries" << endl;
            Wait(Normal(friesFryingPrepareTime.center, friesFryingPrepareTime.scattering));
            (new FryingFries)->Activate(Time);
            friesPreparing++;
            continue;
        }

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