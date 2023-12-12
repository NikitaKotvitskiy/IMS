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
    orderQueue.push(this);
    Passivate();
}