/******************************************************************************
 *                                  MCD
 *                              mcd_kitchen.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Definition of kitchen workers' behavior 
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

using namespace std;

int bunsInToaster       = 0; // Buns which are being prepared in the toater
int bunsReady           = 0; // Ready buns in toater
int bunsOnLine          = 0; // Empty buns on line
int bunsFilled          = 0; // Filled buns in line
int addOrder            = 0; // Count of addition orders
int rawReady            = 0; // Count of ready raw on raw fryer 
int rawInTray           = 0; // Count of ready raw in tray
int rawPreparing        = 0; // Count of preparing portions of raw
int burgerFillers       = 0; // Count of workers which are filling burgers
bool burgerIsPacking    = false; // Is anyone packing a burger at the moment
bool addIsPacking       = false; // Is anyone packink an addition at the moment

int meatReady           = 0; // Ready meat in tray
int preparingMeat       = 0; // Preparing meat
int meatOnGrill         = 0; // Ready meat in grill

// Process of transfering of ready raw to the tray
void KitchenWorker::transferRaw() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready raw to the tray" << endl;
    rawReady--;
    Wait(Normal(rawToTrayTime.center, rawToFryerTime.scattering));
    rawInTray += rawPortionsInFryer;
}

// Process of putting new portion of raw to the fryer
void KitchenWorker::prepareRaw() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is preparing new raw for cooking" << endl;
    rawPreparing++;
    Wait(Normal(rawToFryerTime.center, rawToFryerTime.scattering));
    (new Raw)->Activate(Time);
}

// Process of addition packing
void KitchenWorker::packAnAddition() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing an addition" << endl; 
    addIsPacking = true;
    addOrder--;
    rawInTray--;
    Wait(Normal(addPackingTime.center, addPackingTime.scattering));
    additionsPacked++;
    addIsPacking = false;

    additionsTime(Time - additionOrderTimes.front());
    wholeAdditionsTime(Time - additionOrderTimes.front());
    additionOrderTimes.pop();
}

// Process of burger packing
void KitchenWorker::packABurger() {
    burgerIsPacking = true;

    // If there is only one filled bun on the line or only one meat in tray, worker will pack one burger 
    if (bunsFilled == 1 || bunsFilled > 1 && meatReady == 1) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing one burger" << endl;
        bunsFilled--;
        meatReady--;
        Wait(Normal(singleBurgerPackingTime.center, singleBurgerPackingTime.scattering));
        burgersPacked++;

        burgersTime(Time - burgerOrderTimes.front());
        wholeBurgersTime(Time - burgerOrderTimes.front());
        burgerOrderTimes.pop();
    }

    // If threre are two or more filled buns on the line, worker will pack two burgers
    else {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing two burgers" << endl;
        bunsFilled -= 2;
        meatReady -= 2;
        Wait(Normal(doubleBurgerPackingTime.center, doubleBurgerPackingTime.scattering));
        burgersPacked += 2;

        burgersTime(Time - burgerOrderTimes.front());
        burgerOrderTimes.pop();
        burgersTime(Time - burgerOrderTimes.front());
        burgerOrderTimes.pop();
    }
    burgerIsPacking = false;
}

// Process of filling the burger with garnish
void KitchenWorker::fillABurger() {
    burgerFillers++;
    
    // If threre is only one bun on the line, worker will fill one bun
    if (bunsOnLine == 1) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is filling one burger" << endl;
        bunsOnLine--;
        Wait(Normal(singleBurgerFillingTime.center, singleBurgerFillingTime.scattering));
        bunsFilled++;
    }

    // If threre are two or more buns on the line, worker will fill two buns
    else {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is filling two burgers" << endl;
        bunsOnLine -= 2;
        Wait(Normal(doubleBurgerFillingTime.center, doubleBurgerFillingTime.scattering));
        bunsFilled += 2;
    }
    burgerFillers--;
}

// Process of transgering of ready buns from toater to the line
void KitchenWorker::transferBuns() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready buns to the line" << endl;
    int x = bunsReady;
    bunsReady = 0;
    Wait(Normal(bunsTransferTime.center, bunsTransferTime.scattering));
    bunsOnLine += x;
}

// Process of accepting new orders
void KitchenWorker::prepareBuns() {
    int burgerOrdersAccepted = 0;
    // While order list isn't empty or while the iniciator don't find two orders for burgers
    while(!kitchenOrderQueue.empty() && burgerOrdersAccepted < 2) {
        // Iniciator accepts a new order
        bool orderIsBurger = kitchenOrderQueue.front();
        kitchenOrderQueue.pop();

        // If it is an addition order, iniciator will pass the package for garnish to finishe
        if (!orderIsBurger) {
            if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker takes an order for addition" << endl;
            Wait(Normal(addOrderingTime.center, addOrderingTime.scattering));
            addOrder++;
        }

        // If it is a burger order, iniciator will prepare a bun
        else
            burgerOrdersAccepted++;
    }
    
    // Is iniciator found at leat one burger order, he will put buns to the toaster
    if (burgerOrdersAccepted != 0) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker gets " << burgerOrdersAccepted << " buns in toaster" << endl;
        Wait(Normal(bunsToToasterTime.center, bunsToToasterTime.scattering));
        bunsInToaster += burgerOrdersAccepted;
        for (int i = 0; i < burgerOrdersAccepted; i++)
            (new Bun)->Activate(Time);
    }
}

// Behavior of finisher who only does frying
void FinisherFryer::Behavior() {
    while(true) {
        kitchenWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        kitchenWorkerFree[id] = false;

        // If there is ready raw on the fryer, finisher will transfer it to the tray
        if (rawReady != 0)
            transferRaw();
        
        // If there isn't enough raw in tray, finisher will prepare new one
        if (rawInTray + rawPreparing * rawPortionsInFryer + rawReady * rawPortionsInFryer - addOrder < rawMinimum && rawPreparing < rawFryerCount)
            prepareRaw();
    }
}

// Behavior of finicher who only does packing
void FinisherPacker::Behavior() {
    while(true) {
        kitchenWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        kitchenWorkerFree[id] = false;

        // If anybody is packing an addition, finisher will wait until he ends
        if (addIsPacking)
            continue;
        
        // If there is an addition order and there is a raw in tray, finisher will pack it
        if (addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        // If anyone is packing a burger, finisher will wait until he ends
        if (burgerIsPacking)
            continue;
        
        // If there are filled buns on the line, finisher will pack them
        if (bunsFilled != 0) {
            packABurger();
            continue;
        }

        // If there are emty buns on the line and nobody fills them, finisher will do it
        if (burgerFillers != 0)
            continue;
        if (bunsOnLine != 0)
            fillABurger();
    }
}

// Behavout of single finisher, who is frying raws and packing additions and burgers
void SingleFinisher::Behavior() {
    while(true) {
        kitchenWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        kitchenWorkerFree[id] = false;

        // If there is ready raw on the fryer, finisher will transfer it to the tray
        if (rawReady != 0)
            transferRaw();
        
        // If there isn't enough raw in tray, finisher will prepare new one
        if (rawInTray + rawPreparing * rawPortionsInFryer + rawReady * rawPortionsInFryer - addOrder < rawMinimum && rawPreparing < rawFryerCount) {
            prepareRaw();
            continue;
        }

        // If anybody is packing an addition, finisher will wait until he ends
        if (addIsPacking)
            continue;
        
        // If there is an addition order and there is a raw in tray, finisher will pack it
        if (addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        // If there is an addition order, finisher will pack it
        if (burgerIsPacking)
            continue;
        
        // If there are filled buns on the line, finisher will pack them
        if (bunsFilled != 0) {
            packABurger();
            continue;
        }

        // If there are emty buns on the line and nobody fills them, finisher will do it
        if (burgerFillers != 0)
            continue;
        if (bunsOnLine != 0)
            fillABurger();
    }
}

// Behavior of an assambler
void Assambler::Behavior() {
    while(true) {
        kitchenWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        kitchenWorkerFree[id] = false;

        // If there is an addition order and nobody does packing, assabler will pack it
        if (!addIsPacking && !burgerIsPacking && addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        // If there is a filled bun on the line and nobody does packing, assabler will pack it
        if (!burgerIsPacking && bunsFilled != 0 && meatReady != 0) {
            packABurger();
            continue;
        }

        // If there are empry buns on the line, assembler will start to fill them
        if (bunsOnLine != 0)
            fillABurger();
    }
}

// Behavior of an iniciator
void Iniciator::Behavior() {
    while(true) {
        kitchenWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        kitchenWorkerFree[id] = false;

        // If there are some ready buns in the toaster, iniciator will transfer them to the line
        if (bunsReady != 0) {
            transferBuns();
            continue;
        }

        // If there aren't preparing buns in the toaster, iniciator will start to look for new orders
        if (bunsInToaster == 0 && !kitchenOrderQueue.empty()) {
            prepareBuns();
            continue;
        }

        // If iniciator is the only worker in the line and there is an addition order, iniciator will pack it
        if (burgerFillers == 0 && !addIsPacking && !burgerIsPacking && addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        // If iniciator is the only worker in the line and there is a filled burger, iniciator will pack it
        if (burgerFillers == 0 && !addIsPacking && !burgerIsPacking && bunsFilled != 0 && meatReady != 0) {
            packABurger();
            continue;
        }

        // If there are some empty buns in the line which aren't being filled by anyone, iniciator will start to fill them
        if (bunsOnLine != 0)
            fillABurger();
    }
}

// Behavior of grill worker
void Griller::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        // If there is some ready meat on the grill, worker will transfer it to the tray
        while(meatOnGrill != 0) {
            if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready meat to the tray" << endl;
            meatOnGrill--;
            Wait(Normal(meatTransferTime.center, meatTransferTime.scattering));
            meatReady += meatOnOnePlot;
        }

        // If there isn't enough meat in the tray, worker will start to prepare new meat
        if (meatReady + preparingMeat * meatOnOnePlot - neededMeat < meatMinimum && preparingMeat < grillPlots) {
            if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is preparing new meat" << endl;
            Wait(Normal(meatPreparingStartTime.center, meatPreparingStartTime.scattering));
            neededMeat -= meatOnOnePlot;
            if (neededMeat < 0)
                neededMeat = 0;
            preparingMeat++;
            (new Meat)->Activate(Time);
        }
    }
}

void Bun::Behavior() {
    Wait(bunTime);
    bunsInToaster--;
    bunsReady++;
}

void Raw::Behavior() {
    Wait(rawTime);
    rawPreparing--;
    rawReady++;
}

void Meat::Behavior() {
    Wait(meatTime);
    preparingMeat--;
    meatOnGrill++;
}