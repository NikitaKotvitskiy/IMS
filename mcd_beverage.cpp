#include <iostream>
#include "simlib.h"
#include "mcd.h"

int drinksFinished = 0;
int drinksPreparing = 0;

void BeverageWorker::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (drinksFinished != 0) {
            drinksFinished--;
            Wait(Normal(drinkPackingTime.center, drinkPackingTime.scattering));
            drinksPacked++;
            continue;
        }

        if (drinksOrderCount != 0 && drinkDistributorCount > drinksPreparing) {
            drinksOrderCount--;
            Wait(Normal(drinkPreparingTime.center, drinkPreparingTime.scattering));
            (new Drink)->Activate();
            drinksPreparing++;
        }
    }
}

void Drink::Behavior() {
    Wait(drinkFillingTime);
    drinksPreparing--;
    drinksFinished++;
}