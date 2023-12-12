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
    drinksOrderCount += drinks;
    for (int i = 0; i < burgers; i++) {
        neededMeat++;
        kitchenOrderQueue.push(true);
    }
    for (int i = 0; i < additions; i++)
        kitchenOrderQueue.push(false);
    orderQueue.push(this);
    Passivate();
}