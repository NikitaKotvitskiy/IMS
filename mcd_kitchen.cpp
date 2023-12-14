#include <iostream>
#include "simlib.h"
#include "mcd.h"

using namespace std;

int bunsInToaster = 0;
int bunsReady = 0;
int bunsOnLine = 0;
int bunsFilled = 0;
int addOrder = 0;
int rawFinished = 0;
int rawReady = 0;
int rawInTray = 0;
int rawPreparing = 0;
int burgerFillers = 0;
bool burgerIsPacking = false;
bool addIsPacking = false;
int meatReady = 0;

int preparingMeat = 0;
int meatOnGrill = 0;

void KitchenWorker::transferRaw() {
    while (rawReady != 0) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready raw to the tray" << endl;
        rawReady--;
        Wait(Normal(rawToTrayTime.center, rawToFryerTime.scattering));
        rawInTray++;
    }
}

void KitchenWorker::prepareRaw() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is preparing new raw for cooking" << endl;
    rawPreparing++;
    Wait(Normal(rawToFryerTime.center, rawToFryerTime.scattering));
    (new Raw)->Activate(Time);
}

void KitchenWorker::packAnAddition() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing an addition" << endl; 
    addIsPacking = true;
    addOrder--;
    rawInTray--;
    Wait(Normal(addPackingTime.center, addPackingTime.scattering));
    additionsPacked++;
    addIsPacking = false;
}

void KitchenWorker::packABurger() {
    burgerIsPacking = true;
    if (bunsFilled == 1 || bunsFilled > 1 && meatReady == 1) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing one burger" << endl;
        bunsFilled--;
        meatReady--;
        Wait(Normal(singleBurgerPackingTime.center, singleBurgerPackingTime.scattering));
        burgersPacked++;
    }
    else {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is packing two burgers" << endl;
        bunsFilled -= 2;
        meatReady -= 2;
        Wait(Normal(doubleBurgerPackingTime.center, doubleBurgerPackingTime.scattering));
        burgersPacked += 2;
    }
    burgerIsPacking = false;
}

void KitchenWorker::fillABurger() {
    burgerFillers++;
    if (bunsOnLine == 1) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is filling one burger" << endl;
        bunsOnLine--;
        Wait(Normal(singleBurgerFillingTime.center, singleBurgerFillingTime.scattering));
        bunsFilled++;
    }
    else {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is filling two burgers" << endl;
        bunsOnLine -= 2;
        Wait(Normal(doubleBurgerFillingTime.center, doubleBurgerFillingTime.scattering));
        bunsFilled += 2;
    }
    burgerFillers--;
}

void KitchenWorker::transferBuns() {
    if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready buns to the line" << endl;
    int x = bunsReady;
    bunsReady = 0;
    Wait(Normal(bunsTransferTime.center, bunsTransferTime.scattering));
    bunsOnLine += x;
}

void KitchenWorker::prepareBuns() {
    int burgerOrdersAccepted = 0;
    while(!kitchenOrderQueue.empty() && burgerOrdersAccepted < 2) {
        bool orderIsBurger = kitchenOrderQueue.front();
        kitchenOrderQueue.pop();
        if (!orderIsBurger) {
            if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker takes an order for addition" << endl;
            Wait(Normal(addOrderingTime.center, addOrderingTime.scattering));
            addOrder++;
        }
        else
            burgerOrdersAccepted++;
    }
    
    if (burgerOrdersAccepted != 0) {
        if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker gets " << burgerOrdersAccepted << " buns in toaster" << endl;
        Wait(Normal(bunsToToasterTime.center, bunsToToasterTime.scattering));
        bunsInToaster += burgerOrdersAccepted;
        for (int i = 0; i < burgerOrdersAccepted; i++)
            (new Bun)->Activate(Time);
    }
}

void FinisherFryer::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (rawReady != 0)
            transferRaw();
        
        if (rawInTray + rawPreparing + rawReady - addOrder < rawMinimum && rawPreparing < rawFryerCount)
            prepareRaw();
    }
}

void FinisherPacker::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (addIsPacking)
            continue;
        
        if (addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        if (burgerIsPacking)
            continue;
        
        if (bunsFilled != 0) {
            packABurger();
            continue;
        }

        if (burgerFillers != 0)
            continue;
        
        if (bunsOnLine != 0)
            fillABurger();
    }
}

void SingleFinisher::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (rawReady != 0)
            transferRaw();
        
        if (rawInTray + rawPreparing + rawReady - addOrder < rawMinimum && rawPreparing < rawFryerCount) {
            prepareRaw();
            continue;
        }

        if (addIsPacking)
            continue;
        
        if (addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        if (burgerIsPacking)
            continue;
        
        if (bunsFilled != 0) {
            packABurger();
            continue;
        }

        if (burgerFillers != 0)
            continue;
        
        if (bunsOnLine != 0)
            fillABurger();
    }
}

void Assambler::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (!addIsPacking && !burgerIsPacking && addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        if (!burgerIsPacking && bunsFilled != 0 && meatReady != 0) {
            packABurger();
            continue;
        }

        if (bunsOnLine != 0)
            fillABurger();
    }
}

void Iniciator::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (bunsReady != 0) {
            transferBuns();
            continue;
        }

        if (bunsInToaster == 0 && !kitchenOrderQueue.empty()) {
            prepareBuns();
            continue;
        }

        if (burgerFillers == 0 && !addIsPacking && !burgerIsPacking && addOrder != 0 && rawInTray != 0) {
            packAnAddition();
            continue;
        }

        if (burgerFillers == 0 && !addIsPacking && !burgerIsPacking && bunsFilled != 0 && meatReady != 0) {
            packABurger();
            continue;
        }

        if (bunsOnLine != 0)
            fillABurger();
    }
}

void Griller::Behavior() {
    while (true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        while(meatOnGrill != 0) {
            if (KITCHEN_DEBUG_MODE) cout << Time << ": kitchen worker is transfering ready meat to the tray" << endl;
            meatOnGrill--;
            Wait(Normal(meatTransferTime.center, meatTransferTime.scattering));
            meatReady += meatOnOnePlot;
        }

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