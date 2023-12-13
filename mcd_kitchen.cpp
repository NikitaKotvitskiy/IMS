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
        cout << Time << ": raw is transfering to tray" << endl;
        rawReady--;
        Wait(Normal(rawToTrayTime.center, rawToFryerTime.scattering));
        rawInTray++;
    }
}

void KitchenWorker::prepareRaw() {
    rawPreparing++;
    Wait(Normal(rawToFryerTime.center, rawToFryerTime.scattering));
    (new Raw)->Activate(Time);
    cout << Time << ": raw is starting to prepare" << endl;
}

void KitchenWorker::packAnAddition() {
    addIsPacking = true;
    addOrder--;
    rawInTray--;
    Wait(Normal(addPackingTime.center, addPackingTime.scattering));
    additionsPacked++;
    addIsPacking = false;
    cout << Time << ": addition is being packed" << endl;
}

void KitchenWorker::packABurger() {
    burgerIsPacking = true;
    if (bunsFilled == 1 || bunsFilled > 1 && meatReady == 1) {
        bunsFilled--;
        meatReady--;
        Wait(Normal(singleBurgerPackingTime.center, singleBurgerPackingTime.scattering));
        burgersPacked++;
        cout << Time << ": one burger is being packed" << endl;
    }
    else {
        bunsFilled -= 2;
        meatReady -= 2;
        Wait(Normal(doubleBurgerPackingTime.center, doubleBurgerPackingTime.scattering));
        burgersPacked += 2;
        cout << Time << ": two burgers are being packed" << endl;
    }
    burgerIsPacking = false;
}

void KitchenWorker::fillABurger() {
    burgerFillers++;
    if (bunsOnLine == 1) {
        bunsOnLine--;
        Wait(Normal(singleBurgerFillingTime.center, singleBurgerFillingTime.scattering));
        bunsFilled++;
        cout << Time << ": one burger is being filled" << endl;
    }
    else {
        bunsOnLine -= 2;
        Wait(Normal(doubleBurgerFillingTime.center, doubleBurgerFillingTime.scattering));
        bunsFilled += 2;
        cout << Time << ": two burgers are being filled" << endl;
    }
    burgerFillers--;
}

void KitchenWorker::transferBuns() {
    int x = bunsReady;
    bunsReady = 0;
    Wait(Normal(bunsTransferTime.center, bunsTransferTime.scattering));
    bunsOnLine += x;
    cout << Time << ": " << x << " buns are being transfered to the line" << endl;
}

void KitchenWorker::prepareBuns() {
    int burgerOrdersAccepted = 0;
    while(!kitchenOrderQueue.empty() && burgerOrdersAccepted < 2) {
        bool orderIsBurger = kitchenOrderQueue.front();
        kitchenOrderQueue.pop();
        if (!orderIsBurger) {
            Wait(Normal(addOrderingTime.center, addOrderingTime.scattering));
            cout << Time << ": addition is ordered" << endl;
            addOrder++;
        }
        else
            burgerOrdersAccepted++;
    }
    
    if (burgerOrdersAccepted != 0) {
        Wait(Normal(bunsToToasterTime.center, bunsToToasterTime.scattering));
        cout << Time << ": " << burgerOrdersAccepted << " buns are filled in toater" << endl;
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
            cout << Time << ": meat is transfered to tray" << endl;
            meatOnGrill--;
            Wait(Normal(meatTransferTime.center, meatTransferTime.scattering));
            meatReady += meatOnOnePlot;
        }

        if (meatReady + preparingMeat * meatOnOnePlot - neededMeat < meatMinimum && preparingMeat < grillPlots) {
            Wait(Normal(meatPreparingStartTime.center, meatPreparingStartTime.scattering));
            cout << Time << ": meat is preparing" << endl;
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