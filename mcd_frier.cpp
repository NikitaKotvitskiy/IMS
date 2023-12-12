#include <iostream>
#include "simlib.h"
#include "mcd.h"

void Frier::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (friesFinished != 0) {
            cout << Time << ": Frier is transfering fries..." << endl;
            friesFinished--;
            Wait(Normal(friesTransferTime.center, friesTransferTime.scattering));
            friesReady += friesPortionsInSlot;
            continue;
        }

        if (friesReady - friesOrderCount + friesPreparing * friesPortionsInSlot < minimalFries && friesPreparing < frierSlotsCount) {
            cout << Time << ": Frier is preparing fries..." << endl;
            Wait(Normal(friesFryingPrepareTime.center, friesFryingPrepareTime.scattering));
            (new FryingFries)->Activate(Time);
            friesPreparing++;
            continue;
        }

        if (friesOrderCount != 0) {
            cout << Time << ": Frier is packing fries..." << endl;
            friesOrderCount--;
            friesReady--;
            Wait(Normal(friesPackingTime.center, friesPackingTime.scattering));
            friesPacked++;
        }
    }
}

void FryingFries::Behavior() {
    Wait(friesPreparingTime);
    friesPreparing--;
    friesFinished++;
}