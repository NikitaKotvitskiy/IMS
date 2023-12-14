#include <iostream>
#include "simlib.h"
#include "mcd.h"

using namespace std;

void Client::saveStats() {
    clientDissatisfaction(dissatisfaction);
    clientInMCDTime(Time - arriveTime);
}

int Client::chooseFacility() {
    Wait(orderInCashRegister ? Normal(whichCashRegister.center, whichCashRegister.scattering) : Normal(whichKiosk.center, whichKiosk.scattering));
    int idx = 0;
    Facility * fac = orderInCashRegister ? cashRegisters : kiosks;
    for (int i = 1; i < (orderInCashRegister ? cashRegisterCount : kioskCount); i++)
        if (fac[i].QueueLen() < fac[idx].QueueLen())
            idx = i;
    return idx;
}

void Client::makeAnOrder() {
    bool orderBurger = Random() < orderBurgerChance ? true : false;
    bool orderFries = Random() < orderFriesChance ? true : false;
    bool orderAddition = Random() < orderAdditionChance ? true : false;
    bool orderDrink = Random() < orderDrinkChance ? true : false;

    if (orderBurger) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        burgers++;
    }
    if (orderFries) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        fries++;
    }
    if (orderAddition) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        additions++;
    }
    if (orderDrink) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        drinks++;
    }

    while (orderBurger && Random() < orderAnotherBurgerChance) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        burgers++;
    }
    
    while (orderAddition && Random() < orderAnotherAdditionChance) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        additions++;
    }
    
    while (orderFries && Random() < orderAnotherFriesChance) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        fries++;
    }
    
    while (orderDrink && Random() < orderAnotherDrinkChance) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        drinks++;
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
    Seize(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
    dissatisfaction += int(Time - startWaitTime);
    orderInCashRegister ? cashRegisterQueueTime(Time - startWaitTime) : kioskQueueTime(Time - startWaitTime);
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
        Release(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
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
        Release(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
        saveStats();
        Cancel();
    }
    
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to wait for his order" << endl;
    startWaitTime = Time;
    Passivate();
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has been waiting for his order for " << Time - startWaitTime << " minutes" << endl;
    dissatisfaction += int(Time - startWaitTime);
    orderWaitingTime(Time - startWaitTime);

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