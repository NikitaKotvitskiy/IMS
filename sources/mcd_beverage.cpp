#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

int drinksFinished = 0;
int drinksPreparing = 0;

void BeverageWorker::Behavior() {
    while (true) {
        beverageWorkersFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        beverageWorkersFree[id] = false;

        if (drinksFinished != 0) {
            if (BEVERAGE_DEBUG_MODE) cout << Time << ": beverage worker is packing drink" << endl;
            drinksFinished--;
            Wait(Normal(drinkPackingTime.center, drinkPackingTime.scattering));
            drinksPacked++;

            drinksTime(Time - drinksOrderTimes.front());
            drinksOrderTimes.pop();

            continue;
        }

        if (drinksOrderCount != 0 && drinkDistributorCount > drinksPreparing) {
            if (BEVERAGE_DEBUG_MODE) cout << Time << ": beverage worker is preparing drink" << endl;
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