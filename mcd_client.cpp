#include <iostream>
#include "simlib.h"
#include "mcd.h"

using namespace std;

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
    while(state != ORDER_END) {
        Wait(orderInCashRegister ? Normal(whatCashRegister.center, whatCashRegister.scattering) : Normal(whatKiosk.center, whatKiosk.scattering));
        double chance = Random();
        switch(state) {
            case ORDER_START:
                if (chance < orderBurgerFirst) {
                    burgers++;
                    state = ORDER_BURGERS;
                }
                else if (chance < orderBurgerFirst + orderAdditionFirst) {
                    additions++;
                    state = ORDER_ADDITIONS;
                }
                else if (chance < orderBurgerFirst + orderAdditionFirst + orderFriesFirst) {
                    fries++;
                    state = ORDER_FRIES;
                }
                else if (chance < orderBurgerFirst + orderAdditionFirst + orderFriesFirst + orderDrinkFirst) {
                    drinks++;
                    state = ORDER_DRINKS;
                }
                else
                    state = ORDER_END;
                break;
            case ORDER_BURGERS:
                if (chance < orderAnotherBurger)
                    burgers++;
                else if (chance < orderAnotherBurger + orderAdditionSecond) {
                    additions++;
                    state = ORDER_ADDITIONS;
                }
                else if (chance < orderAnotherBurger + orderAdditionSecond + orderFriesSecond) {
                    fries++;
                    state = ORDER_FRIES;
                }
                else if (chance < orderAnotherBurger + orderAdditionSecond + orderFriesSecond + orderDrinkSecond) {
                    drinks++;
                    state = ORDER_DRINKS;
                }
                else
                    state = ORDER_END;
                break;
            case ORDER_ADDITIONS:
                if (chance < orderAnotherAddition)
                    additions++;
                else if (chance < orderAnotherAddition + orderFriesThird) {
                    fries++;
                    state = ORDER_FRIES;
                }
                else if (chance < orderAnotherAddition + orderFriesThird + orderDrinkThird) {
                    drinks++;
                    state = ORDER_DRINKS;
                }
                else
                    state = ORDER_END;
                break;
            case ORDER_FRIES:
                if (chance < orderAnotherFries)
                    fries++;
                else if (chance < orderAnotherFries + orderDrinkLast) {
                    drinks++;
                    state = ORDER_DRINKS;
                }
                else
                    state = ORDER_END;
                break;
            case ORDER_DRINKS:
                if (chance < orderAnotherDrink)
                    drinks++;
                else
                    state = ORDER_END;
                break;
        }
    }
}

void Client::Behavior() {
    clientNumber = clientCounter;
    clientCounter++;
    Wait(Normal(whereToOrderTime.center, whereToOrderTime.scattering));
    orderInCashRegister = true;
    if (Random() < kioskChance)
            orderInCashRegister = false;

    cout << Time << ": client " << clientNumber << " has decided to order in " << (orderInCashRegister ? "cash register" : "kiosk") << endl;

    int idx = chooseFacility();

    cout << Time << ": client " << clientNumber << " has chosen " << (orderInCashRegister ? "cash register " : "kiosk ") << idx << endl;

    startWaitTime = Time;
    Seize(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);

    cout << Time << ": client " << clientNumber << " was in queue for " << Time - startWaitTime << endl;

    dissatisfaction += int(Time - startWaitTime);
    makeAnOrder();

    cout << Time << ": client " << clientNumber << " has made an oder:" << endl;
    cout << "\t" << burgers << " burgers" << endl;
    cout << "\t" << additions << " additions" << endl;
    cout << "\t" << fries << " fries" << endl;
    cout << "\t" << drinks << " drinks" << endl;

    if (burgers + additions + fries + drinks != 0) {
        Wait(Normal(payTime.center, payTime.scattering));
        Release(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
        if (Random() < payFailed) {
            cout << Time << ": client " << clientNumber << " cannot pay for his order" << endl;
            cout << Time << ": client " << clientNumber << " leaves" << endl;
            clientsLeft++;
            Cancel();
        }
        else {
            cout << Time << ": client " << clientNumber <<  " has paid for his order" << endl;
            packOrder = true;
            if (Random() < eatInMCD)
                packOrder = false;
            cout << Time << ": client " << clientNumber << " wants to " << (packOrder ? "pack his order" : "eat in MCD") << endl;
            order = new Order(burgers, additions, fries, drinks, packOrder, this);
            order->Activate(Time);
        }
    }
    else {
        clientsLeft++;
        cout << Time << ": client " << clientNumber << " has not chosen anything" << endl;
        cout << Time << ": client " << clientNumber << " leaves" << endl;
        Release(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
        Cancel();
    }
    
    cout << Time << ": client " << clientNumber << " starts to wait for his order" << endl;
    startWaitTime = Time;
    Passivate();
    cout << Time << ": client " << clientNumber << " waited for his order for " << int(Time - startWaitTime) << " minutes" << endl;
    dissatisfaction += int(Time - startWaitTime);

    Wait(Normal(pickOrderTime.center, pickOrderTime.scattering));
    if (packOrder) {
        clientsLeft++;
        cout << Time << ": client " << clientNumber << " has got his packed order and leaves" << endl;
        Cancel();
    }
    cout << Time << ": client " << clientNumber << " has got his order and starts to look for a table" << endl;
    tableFound = false;
    startWaitTime = Time;
    for (int i = 0; i < tableCount; i++) {
        Wait(Normal(searchTableTime.center, searchTableTime.scattering));
        if (tables[i].busy) {
            cout << Time << ": client " << clientNumber << " is expecting table " << i << " and it is busy" << endl;
            continue;
        }
        if (!tables[i].busy && tables[i].dirty) {
            cout << Time << ": client " << clientNumber << " is expecting table " << i << " and it is dirty" << endl;
            dissatisfaction += dirtyTableInf;
            continue;
        }
        if (Random() < denyTable) {
            cout << Time << ": client " << clientNumber << " is expecting table " << i << " and he does not like it" << endl;
            continue;
        }
        cout << Time << ": client " << clientNumber << " is expecting table " << i << " and it is good!" << endl;
        tableFound = true;
        chosenTable = i;
        tables[i].busy = true;
        break;
    }

    if (!tableFound) {
        cout << Time << ": client " << clientNumber << " did not found table and now wants to pack his order" << endl;
        dissatisfaction += noTableInf;
        startWaitTime = Time;
        extradition.Insert(this);
        Passivate();
        dissatisfaction += int(Time - startWaitTime);
        clientsLeft++;
        Cancel();
    }

    for (int i = 0; i < burgers + additions + fries + drinks; i++) {
        if (Random() < cancelMeal) {
            cout << Time << ": client " << clientNumber << " does not want to finish food" << endl;
            tables[chosenTable].busy = false;
            if (Random() < packRemains) {
                cout << Time << ": client " << clientNumber << " wants to pack remains" << endl;
                startWaitTime = Time;
                extradition.Insert(this);
                Passivate();
                dissatisfaction += int(Time - startWaitTime);
                clientsLeft++;
                cout << Time << ": client " << clientNumber << " leaves" << endl;
                Cancel();
            }
            if (Random() < badClient) {
                cout << Time << ": client " << clientNumber << " client leaves the food on table" << endl;
                tables[chosenTable].dirty = true;
                clientsLeft++;
                cout << Time << ": client " << clientNumber << " leaves" << endl;
                Cancel();
            }
            clientsLeft++;
            cout << Time << ": client " << clientNumber << " cleaned the table" << endl;
            cout << Time << ": client " << clientNumber << " leaves" << endl;
            Cancel();
        }
        cout << Time << ": client " << clientNumber << " eats an item " << i << endl;
        Wait(Normal(eatOneItemTime.center, eatOneItemTime.scattering));
    }

    cout << Time << ": client " << clientNumber << " finishes eating " << endl;
    tables[chosenTable].busy = false;
    if (Random() < badClient)
        tables[chosenTable].dirty = true;
    
    cout << Time << ": client " << clientNumber << (tables[chosenTable].dirty ? " leaves the trash" : " clean the table") << endl;
    cout << Time << ": client " << clientNumber << " leaves" << endl;
    clientsLeft++;
}