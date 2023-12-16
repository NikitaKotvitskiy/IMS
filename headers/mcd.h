/******************************************************************************
 *                                  MCD
 *                                 mcd.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: This header contains the declaration of variables, which describe
 *               state of the restaurant, like count of different workers, statistic,
 *               count of ready production and so on
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

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

//
// Debugging parameters
//

const bool CLIENT_DEBUG_MODE = false;
const bool KITCHEN_DEBUG_MODE = false;
const bool SERVICE_DEBUG_MODE = false;
const bool FRIES_DEBUG_MODE = false;
const bool LOBBY_DEBUG_MODE = false;
const bool BEVERAGE_DEBUG_MODE = false;
extern int clientCounter;

// This strucrure stores the parameters of different time intervals of experiment
struct ExperimentInterval {
    double intervalLength;  // How long is the interval
    double clientFrequency; // How often will come new clients
    int lobbyWorkers;       // Count of workers on lobby
    int serviceWorkers;     // Count of workers on service
    int beverageWorkers;    // Count of workers on beverage
    int kitchenWorkers;     // Count of workers in kitchen
    int friesCount;         // Normal count of ready fries
    int meatCount;          // Normal count of ready meat
    int rawCount;           // Normal count of ready raw
};

// This structure describes state of a table in MCD
struct Table {
    bool busy = false;
    bool dirty = false;
    bool inService = false;
};

// Class of experiment
class Experiment : public Process {
public:
    int experimentCounter = 1;
    void Behavior();
};

// Queue of intervals of experiment
extern queue<ExperimentInterval> experiment;

// Event, which is generating new clients
class ClientGenerator : public Event {
    void Behavior();
};

// Reads the experiment parameters from experiment.txt
void readExperiment();

// The whole time of experiment
extern double experimentTime;

// Functions for statistics
void printStats();
void setStatNames(int intervalNum);

// Working facilities of system
extern vector<Facility *> cashRegisters;
extern vector<Facility *> kiosks;
extern vector<Table> tables;
extern queue<Client *> extradition;

// Worker parameters
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
extern int drinksOrderCount;
extern int neededMeat;

// Queues of different products orders
extern queue<Order *> orderQueue;
extern queue<Order *> packedOrderQueue;
extern queue<bool> kitchenOrderQueue;

// Time of products preparation start
extern queue<double> burgerOrderTimes;
extern queue<double> additionOrderTimes;
extern queue<double> orderPackExtraditionTimes;
extern queue<double> friesOrderTimes;
extern queue<double> drinksOrderTimes;

// Is there an extraditor on the service
extern bool isExtraditor;

//
// Statistics
//

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