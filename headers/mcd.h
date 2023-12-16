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
const bool CLIENT_DEBUG_MODE = false;
const bool KITCHEN_DEBUG_MODE = false;
const bool SERVICE_DEBUG_MODE = false;
const bool FRIES_DEBUG_MODE = false;
const bool LOBBY_DEBUG_MODE = false;
const bool BEVERAGE_DEBUG_MODE = false;
extern int clientCounter;

struct ExperimentInterval {
    double intervalLength;
    double clientFrequency;
    int lobbyWorkers;
    int serviceWorkers;
    int beverageWorkers;
    int kitchenWorkers;
    int friesCount;
    int meatCount;
    int rawCount;
};

extern queue<ExperimentInterval> experiment;
void readExperiment();
extern double experimentTime;
void printStats();
void setStatNames(int intervalNum);

// This structure describes state of a table in MCD
struct Table {
    bool busy = false;
    bool dirty = false;
    bool inService = false;
};

extern vector<Facility *> cashRegisters;
extern vector<Facility *> kiosks;
extern vector<Table> tables;
extern queue<Client *> extradition;

extern int lobbyWorkerCount;
extern vector<bool> lobbyWorkerFree;
extern vector<LobbyWorker *> lobbyWorkersVec;
extern int serviceWorkerCount;
extern vector<bool> serviceWorkerFree;
extern vector<ServiceWorker * > serviceWorkersVec;
extern int beverageWorkerCount;
extern vector<bool> beverageWorkersFree;
extern vector<BeverageWorker * > beverageWorkersVec;
extern int kitchenWorkerCount;
extern vector<bool> kitchenWorkerFree;
extern vector<KitchenWorker *> kitchenWorkersVec;

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
extern queue<double> orderPackExtraditionTimes;
extern int neededMeat;

extern bool isExtraditor;

class ClientGenerator : public Event {
    void Behavior();
};

extern Stat clientInMCDTime;
extern Stat wholeClientInMCDTime;

extern Stat cashRegisterQueueTime;
extern Stat wholeCashRegisterQueueTime;

extern Stat kioskQueueTime;
extern Stat wholeKioskQueueTime;

extern Stat orderWaitingTime;
extern Stat wholeOrderWaitingTime;

extern Stat tableSearchingTime;
extern Stat wholeTtableSearchingTime;

extern Stat clientDissatisfaction;
extern Stat wholeClientDissatisfaction;

extern Stat burgersTime;
extern Stat wholeBurgersTime;

extern Stat friesTime;
extern Stat wholeFriesTime;

extern Stat drinksTime;
extern Stat wholeDrinksTime;

extern Stat additionsTime;
extern Stat wholeAdditionsTime;

extern Stat orderExtraditigTime;
extern Stat wholeOrderExtraditionTime;

#endif