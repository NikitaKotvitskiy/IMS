#ifndef MCD_H
#define MCD_H

#include <iostream>
#include <queue>
#include "simlib.h"
#include "mcd_params.h"
#include "mcd_order.h"
#include "mcd_client.h"
#include "mcd_lobby_worker.h"
#include "mcd_service_worker.h"
#include "mcd_frier.h"
#include "mcd_beverage.h"

using namespace std;

extern int clientsLeft;        // TMP
extern int clientCounter;      // TMP

struct Table {
    bool busy = false;
    bool dirty = false;
    bool inService = false;
};

extern Facility cashRegisters[cashRegisterCount];
extern Facility kiosks[kioskCount];
extern Queue extradition;
extern Table tables[tableCount];

extern int friesPacked;
extern int burgersPacked;
extern int additionsPacked;
extern int drinksPacked;

extern int friesOrderCount;
extern int friesReady;
extern int friesPreparing;
extern int friesFinished;

extern int drinksOrderCount;
extern int drinksFinished;
extern int drinksPreparing;

extern queue<Order *> orderQueue;
extern queue<Order *> packedOrderQueue;

extern bool isExtraditor;

class ClientGenerator : public Event {
    void Behavior();
};

void initMCD();

#endif