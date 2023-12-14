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
const bool BEVERAGE_DEBUG_MODE = true;
extern int clientCounter;

// This structure describes state of a table in MCD
struct Table {
    bool busy = false;
    bool dirty = false;
    bool inService = false;
};

extern Facility cashRegisters[cashRegisterCount];
extern Facility kiosks[kioskCount];
extern queue<Client *> extradition;
extern Table tables[tableCount];

// Ready production
extern int friesPacked;
extern int burgersPacked;
extern int additionsPacked;
extern int drinksPacked;

// Needed production
extern int friesOrderCount;
extern queue<double> friesOrderTimes;

extern int drinksOrderCount;
extern queue<double> drinksOrderTimes;

extern queue<Order *> orderQueue;
extern queue<Order *> packedOrderQueue;
extern queue<bool> kitchenOrderQueue;
extern queue<double> burgerOrderTimes;
extern queue<double> additionOrderTimes;
extern int neededMeat;

extern bool isExtraditor;

class ClientGenerator : public Event {
    void Behavior();
};

void initMCD();

// Client statistics
extern Stat clientInMCDTime;
extern Stat cashRegisterQueueTime;
extern Stat kioskQueueTime;
extern Stat orderWaitingTime;
extern Stat clientDissatisfaction;
extern Stat tableSearchingTime;

// Kitchen statistics
extern Stat burgersTime;
extern Stat friesTime;
extern Stat drinksTime;
extern Stat additionsTime;

#endif