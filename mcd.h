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
#include "mcd_kitchen.h"

using namespace std;

// Debugging parameters
const bool CLIENT_DEBUG_MODE = true;
const bool KITCHEN_DEBUG_MODE = true;
const bool SERVICE_DEBUG_MODE = true;
const bool FRIES_DEBUG_MODE = true;
const bool LOBBY_DEBUG_MODE = true;
extern int clientCounter;


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
extern int drinksOrderCount;

extern queue<Order *> orderQueue;
extern queue<Order *> packedOrderQueue;
extern queue<bool> kitchenOrderQueue;
extern int neededMeat;

extern bool isExtraditor;

class ClientGenerator : public Event {
    void Behavior();
};

void initMCD();

extern Stat clientInMCDTime;
extern Stat cashRegisterQueueTime;
extern Stat kioskQueueTime;
extern Stat orderWaitingTime;
extern Stat clientDissatisfaction;
extern Stat tableSearchingTime;

#endif