/******************************************************************************
 *                                  MCD
 *                              mcd_main.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Definition of beverage workers' behavior
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

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

        // If some drink is ready, worker will pack it
        if (drinksFinished != 0) {
            if (BEVERAGE_DEBUG_MODE) cout << Time << ": beverage worker is packing drink" << endl;
            drinksFinished--;
            Wait(Normal(drinkPackingTime.center, drinkPackingTime.scattering));
            drinksPacked++;

            drinksTime(Time - drinksOrderTimes.front());
            wholeDrinksTime(Time - drinksOrderTimes.front());
            drinksOrderTimes.pop();

            continue;
        }

        // If there is a drink order, worker will start to fill a new drink 
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