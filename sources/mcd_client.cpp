/******************************************************************************
 *                                  MCD
 *                              mcd_main.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Definition of behavior of client
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

using namespace std;

// Saves the statistics of client before leaving
void Client::saveStats() {
    clientDissatisfaction(dissatisfaction);
    wholeClientDissatisfaction(dissatisfaction);
    clientInMCDTime(Time - arriveTime);
    wholeClientInMCDTime(Time - arriveTime);
}

// Returns an index of cash register or kiosk depending on lengths of queues
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

// Generates an order: chooses it's type and number of different products
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

// Behavour of client
void Client::Behavior() {
    clientNumber = clientCounter++;
    arriveTime = Time;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has entered the MCD" << endl;

    // Client decides, where he wants to make his order
    Wait(Normal(whereToOrderTime.center, whereToOrderTime.scattering));
    orderInCashRegister = true;
    if (Random() < kioskChance)
        orderInCashRegister = false;

    // Client decides, which facility he wants to use to make his order
    int idx = chooseFacility();
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen to order in " << (orderInCashRegister ? "cash register " : "kiosk ") << endl;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen " << (orderInCashRegister ? "cash register " : "kiosk ") << idx << endl;

    // Client gets in queue
    startWaitTime = Time;
    Seize(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));
    dissatisfaction += int(Time - startWaitTime);

    // Cleint choses, what he wants to order
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

    // If client has chosen anything, he will register his order
    if (burgers + additions + fries + drinks != 0) {
        // Client pays his order
        Wait(Normal(payTime.center, payTime.scattering));
        Release(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));

        // If client has a problem with paying, he cancels his order and leaves
        if (Random() < payFailed) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " cannot pay, so he leaves" << endl;
            saveStats();
            Cancel();
        }
        // In case of successful payment, order is registered and the client decides, where he wants to eat
        else {
            packOrder = true;
            if (Random() < eatInMCD)
                packOrder = false;
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " wants " << (packOrder ? "his order packed" : "to eat in MCD") << endl;
            order = new Order(burgers, additions, fries, drinks, packOrder, this);
            order->Activate(Time);
        }
    }
    // If client didn't chose anything, he leaves
    else {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " didn't choose anything, so he leaves" << endl;
        Release(orderInCashRegister ? *(cashRegisters[idx]) : *(kiosks[idx]));
        saveStats();
        Cancel();
    }
    
    // Client starts to wait for his order
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to wait for his order" << endl;
    startWaitTime = Time;
    Passivate();

    // Client gets his order
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has been waiting for his order for " << Time - startWaitTime << " minutes" << endl;
    dissatisfaction += int(Time - startWaitTime);
    orderWaitingTime(Time - startWaitTime);
    wholeOrderWaitingTime(Time - startWaitTime);
    Wait(Normal(pickOrderTime.center, pickOrderTime.scattering));

    // If client doesn't want to eat in the restaurant, he leaves
    if (packOrder) {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has got his packed order and leaves" << endl;
        saveStats();
        Cancel();
    }

    // If client wants to eat in MCD, he starts to choose a table
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to look for a free cleen table" << endl;
    tableFound = false;
    startWaitTime = Time;

    // He will inspect all tables
    for (int i = 0; i < tableCount; i++) {
        Wait(Normal(searchTableTime.center, searchTableTime.scattering));
        // If table is busy, client can't use it
        if (tables[i].busy) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but it is busy" << endl;
            continue;
        }

        // If table is busy, but dirty, client will deny it 
        if (!tables[i].busy && tables[i].dirty) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but it is dirty" << endl;
            dissatisfaction += dirtyTableInf;
            continue;
        }

        // If client doesn't like tha table, he will try to find another one
        if (Random() < denyTable) {
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " looks at table " << i << ", but he doesn't like it" << endl;
            continue;
        }

        // If client likes clean free table, he will sit there
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " has chosen table " << i << endl;
        tableFound = true;
        chosenTable = i;
        tables[i].busy = true;
        tableSearchingTime(Time - startWaitTime);
        wholeTtableSearchingTime(Time - startWaitTime);
        break;
    }

    // If client didn't find table, he will ask to pack his order
    if (!tableFound) {
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " cannot find a table, so wants his order packed" << endl;
        dissatisfaction += noTableInf;
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " asks to pack his order" << endl;

        // Client waits while his order is being packed
        startWaitTime = Time;
        extradition.push(this);
        Passivate();

        // When clint gets his packed order, he leaves
        dissatisfaction += int(Time - startWaitTime);
        if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " got his packed order and leaves" << endl;
        saveStats();
        Cancel();
    }

    // If client found a table, he starts to eat
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " starts to eat";
    for (int i = 0; i < burgers + additions + fries + drinks; i++) {

        // Client may want to stop eating
        if (Random() < cancelMeal) {
            tables[chosenTable].busy = false;
            // If client wants to eat the remains in future, he will ask to pack the remains
            if (Random() < packRemains) {
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he goes to ask to pack the remains" << endl;
                
                // Client waits while his order is being packed
                startWaitTime = Time;
                extradition.push(this);
                Passivate();

                // When client gets his order, he leaves
                dissatisfaction += int(Time - startWaitTime);
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " got his packed remains, so he leaves" << endl;
                saveStats();
                Cancel();
            }
            
            // If client doesn't want to eat the remains in future, he may leave the remains on the table, so the table remains dirty
            if (Random() < badClient) {
                if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he leaves and doesn't clean the table" << endl;
                tables[chosenTable].dirty = true;

                // Client leaves
                saveStats();
                Cancel();
            }

            // Or he can clean the table and then leave
            if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " doesn't want to eat remains, so he cleans the table and leaves" << endl;
            saveStats();
            Cancel();
        }
        Wait(Normal(eatOneItemTime.center, eatOneItemTime.scattering));
    }
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << " finished eating" << endl;

    // Client can remain the trash on table, so the table will be dirty, or clean it
    tables[chosenTable].busy = false;
    if (Random() < badClient)
        tables[chosenTable].dirty = true;
    if (CLIENT_DEBUG_MODE) cout << Time << ": client " << clientNumber << (tables[chosenTable].dirty ? " didn't clean " : " cleaned ") << "the table and leaves" << endl;

    // Client leaves the restaurant
    saveStats();
}