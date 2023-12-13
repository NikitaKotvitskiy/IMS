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

    int idx = chooseFacility();
    startWaitTime = Time;
    Seize(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
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
            clientsLeft++;
            Cancel();
        }
        else {
            packOrder = true;
            if (Random() < eatInMCD)
                packOrder = false;
            order = new Order(burgers, additions, fries, drinks, packOrder, this);
            order->Activate(Time);
        }
    }
    else {
        clientsLeft++;
        Release(orderInCashRegister ? cashRegisters[idx] : kiosks[idx]);
        Cancel();
    }
    
    startWaitTime = Time;
    Passivate();
    dissatisfaction += int(Time - startWaitTime);

    Wait(Normal(pickOrderTime.center, pickOrderTime.scattering));
    if (packOrder) {
        clientsLeft++;
        Cancel();
    }

    tableFound = false;
    startWaitTime = Time;
    for (int i = 0; i < tableCount; i++) {
        Wait(Normal(searchTableTime.center, searchTableTime.scattering));
        if (tables[i].busy) {
            continue;
        }
        if (!tables[i].busy && tables[i].dirty) {
            dissatisfaction += dirtyTableInf;
            continue;
        }
        if (Random() < denyTable) {
            continue;
        }
        tableFound = true;
        chosenTable = i;
        tables[i].busy = true;
        break;
    }

    if (!tableFound) {
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
            tables[chosenTable].busy = false;
            if (Random() < packRemains) {
                startWaitTime = Time;
                extradition.Insert(this);
                Passivate();
                dissatisfaction += int(Time - startWaitTime);
                clientsLeft++;
                Cancel();
            }
            if (Random() < badClient) {
                tables[chosenTable].dirty = true;
                clientsLeft++;
                Cancel();
            }
            clientsLeft++;
            Cancel();
        }
        Wait(Normal(eatOneItemTime.center, eatOneItemTime.scattering));
    }

    tables[chosenTable].busy = false;
    if (Random() < badClient)
        tables[chosenTable].dirty = true;

    clientsLeft++;
}