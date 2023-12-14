#include <iostream>
#include "simlib.h"
#include "mcd.h"

using namespace std;

Order::Order(int b, int a, int f, int d, bool p, Client * c) {
    burgers = b;
    additions = a;
    fries = f;
    drinks = d;
    pack = p;
    client = c;
}

void Order::Behavior() {
    friesOrderCount += fries;
    for (int i = 0; i < fries; i++)
        friesOrderTimes.push(Time);

    drinksOrderCount += drinks;
    for (int i = 0; i < drinks; i++)
        drinksOrderTimes.push(Time);

    for (int i = 0; i < burgers; i++) {
        neededMeat++;
        kitchenOrderQueue.push(true);
        burgerOrderTimes.push(Time);
    }
    for (int i = 0; i < additions; i++) {
        kitchenOrderQueue.push(false);
        additionOrderTimes.push(Time);
    }
    orderQueue.push(this);
    Passivate();
}