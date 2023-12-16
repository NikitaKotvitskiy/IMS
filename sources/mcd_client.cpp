#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

using namespace std;

void Client::saveStats() {
    clientDissatisfaction(dissatisfaction);
    wholeClientDissatisfaction(dissatisfaction);
    clientInMCDTime(Time - arriveTime);
    wholeClientInMCDTime(Time - arriveTime);
}

int Client::chooseFacility() {
    Wait(orderInCashRegister ? Normal(whichCashRegister.center, whichCashRegister.scattering) : Normal(whichKiosk.center, whichKiosk.scattering));
    int idx = 0;
    if (orderInCashRegister) {
        int min = cashRegisters[0]->Busy() ? 1 : 0;
        for (int i = 1; i < cashRegisterCount; i++)
            if (cashRegisters[i]->QueueLen() + (cashRegisters[i]->Busy() ? 1 : 0) < min) {
                idx = i;
                min = cashRegisters[i]->QueueLen() + (cashRegisters[i]->Busy() ? 1 : 0);
            }
    }
    else {
        int min = kiosks[0]->Busy() ? 1 : 0;
        for (int i = 1; i < kioskCount; i++)
            if (kiosks[i]->QueueLen() + (kiosks[i]->Busy() ? 1 : 0) < min) {
                idx = i;
                min = kiosks[i]->QueueLen() + (kiosks[i]->Busy() ? 1 : 0);
            }
    }
    return idx;
}

enum OrderType {
    SMALL,
    MIDDLE,
    BIG
};

void Client::makeAnOrder() {
    Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
    OrderType orderType;
    double rand = Random();
    if (rand < smallOrderChance) {
        orderType = SMALL;
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen to make a small order" << endl;
    }
    else if (rand < smallOrderChance + middleOrderChance) {
        orderType = MIDDLE;
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen to make a middle order" << endl;
    }
    else {
        orderType = BIG;
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen to make a big order" << endl;
    }

    switch (orderType) {
        case SMALL:
            burgers = Uniform(smallOrderPossibleBurgerCountMin, smallOrderPossibleBurgerCountMax);
            additions = Uniform(smallOrderPossibleFriesCountMin, smallOrderPossibleFriesCountMax);
            fries = Uniform(smallOrderPossibleFriesCountMin, smallOrderPossibleFriesCountMax);
            drinks = Uniform(smallOrderPossibleDrinkCountMin, smallOrderPossibleDrinkCountMax);
            break;
        case MIDDLE:
            burgers = Uniform(middleOrderPossibleBurgerCountMin, middleOrderPossibleBurgerCountMax);
            additions = Uniform(middleOrderPossibleFriesCountMin, middleOrderPossibleFriesCountMax);
            fries = Uniform(middleOrderPossibleFriesCountMin, middleOrderPossibleFriesCountMax);
            drinks = Uniform(middleOrderPossibleDrinkCountMin, middleOrderPossibleDrinkCountMax);
            break;
        case BIG:
            burgers = Uniform(bigOrderPossibleBurgerCountMin, bigOrderPossibleBurgerCountMax);
            additions = Uniform(bigOrderPossibleFriesCountMin, bigOrderPossibleFriesCountMax);
            fries = Uniform(bigOrderPossibleFriesCountMin, bigOrderPossibleFriesCountMax);
            drinks = Uniform(bigOrderPossibleDrinkCountMin, bigOrderPossibleDrinkCountMax);
            break;
    } 
}

void Client::Behavior() {
    clientNumber = clientCounter++;
    arriveTime = Time;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has entered the MCD" << endl;

    Wait(Normal(whereToOrderTime.center, whereToOrderTime.scattering));
    orderInCashRegister = true;
    if (Random() < kioskChance)
        orderInCashRegister = false;
    int idx = chooseFacility();
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen to order in " << (orderInCashRegister ? "cash register " : "kiosk ") << endl;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen " << (orderInCashRegister ? "cash register " : "kiosk ") << idx << endl;

    startWaitTime = Time;
    Seize(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));
    dissatisfaction += int(Time - startWaitTime);
    orderInCashRegister ? cashRegisterQueueTime(Time - startWaitTime) : kioskQueueTime(Time - startWaitTime);
    orderInCashRegister ? wholeCashRegisterQueueTime(Time - startWaitTime) : wholeKioskQueueTime(Time - startWaitTime);
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " was in queue for " << Time - startWaitTime << " minutes" << endl;

    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts ordering" << endl;
    makeAnOrder();

    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has finished ordering:" << endl;
    if (CLIENT_DEBUG_MODE) cout << "\t" << burgers << " burgers" << endl;
    if (CLIENT_DEBUG_MODE) cout << "\t" << additions << " additions" << endl;
    if (CLIENT_DEBUG_MODE) cout << "\t" << fries << " fries" << endl;
    if (CLIENT_DEBUG_MODE) cout << "\t" << drinks << " drinks" << endl;

    if (burgers + additions + fries + drinks != 0) {
        Wait(Normal(payTime.center, payTime.scattering));
        Release(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));
        if (Random() < payFailed) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " cannot pay, so he leaves" << endl;
            saveStats();
            Cancel();
        }
        else {
            packOrder = true;
            if (Random() < eatInMCD)
                packOrder = false;
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " wants " << (packOrder ? "his order packed" : "to eat in MCD") << endl;
            order = new Order(burgers, additions, fries, drinks, packOrder, this);
            order->Activate(Time);
        }
    }
    else {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " didn't choose anything, so he leaves" << endl;
        Release(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));
        saveStats();
        Cancel();
    }
    
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to wait for his order" << endl;
    startWaitTime = Time;
    Passivate();
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has been waiting for his order for " << Time - startWaitTime << " minutes" << endl;
    dissatisfaction += int(Time - startWaitTime);
    orderWaitingTime(Time - startWaitTime);
    wholeOrderWaitingTime(Time - startWaitTime);

    Wait(Normal(pickOrderTime.center, pickOrderTime.scattering));
    if (packOrder) {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has got his packed order and leaves" << endl;
        saveStats();
        Cancel();
    }

    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to look for a free cleen table" << endl;
    tableFound = false;
    startWaitTime = Time;
    for (int i = 0; i < tableCount; i++) {
        Wait(Normal(searchTableTime.center, searchTableTime.scattering));
        if (tables[i].busy) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but it is busy" << endl;
            continue;
        }
        if (!tables[i].busy && tables[i].dirty) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but it is dirty" << endl;
            dissatisfaction += dirtyTableInf;
            continue;
        }
        if (Random() < denyTable) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but he doesn't like it" << endl;
            continue;
        }
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen table " << i << endl;
        tableFound = true;
        chosenTable = i;
        tables[i].busy = true;
        tableSearchingTime(Time - startWaitTime);
        wholeTtableSearchingTime(Time - startWaitTime);
        break;
    }

    if (!tableFound) {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " cannot find a table, so wants his order packed" << endl;
        dissatisfaction += noTableInf;

        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " asks to pack his order" << endl;
        startWaitTime = Time;
        extradition.push(this);
        Passivate();
        dissatisfaction += int(Time - startWaitTime);
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " got his packed order and leaves" << endl;
        saveStats();
        Cancel();
    }

    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to eat";
    for (int i = 0; i < burgers + additions + fries + drinks; i++) {
        if (Random() < cancelMeal) {
            tables[chosenTable].busy = false;
            if (Random() < packRemains) {
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he goes to ask to pack the remains" << endl;
                startWaitTime = Time;
                extradition.push(this);
                Passivate();
                dissatisfaction += int(Time - startWaitTime);
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " got his packed remains, so he leaves" << endl;
                saveStats();
                Cancel();
            }
            if (Random() < badClient) {
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he leaves and doesn't clean the table" << endl;
                tables[chosenTable].dirty = true;
                saveStats();
                Cancel();
            }
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he cleans the table and leaves" << endl;
            saveStats();
            Cancel();
        }
        Wait(Normal(eatOneItemTime.center, eatOneItemTime.scattering));
    }
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " finished eating" << endl;

    tables[chosenTable].busy = false;
    if (Random() < badClient)
        tables[chosenTable].dirty = true;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << (tables[chosenTable].dirty ? " didn't clean " : " cleaned ") << "the table and leaves" << endl;

    saveStats();
}