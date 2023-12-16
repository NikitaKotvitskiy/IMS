/******************************************************************************
 *                                  MCD
 *                                 mcd.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Contains the definition of variables, which describe state of 
 *               the restaurant, like count of different workers, statistic,
 *               count of ready production and so on
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#include "simlib.h"
#include "../headers/mcd.h"
#include <queue>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int clientCounter = 0;

// Working facilities of system
vector<Facility *> cashRegisters;
vector<Facility *> kiosks;
vector<Table> tables;
queue<Client *> extradition;

// Worker parameters
int lobbyWorkerCount = 0;
vector<bool> lobbyWorkerFree;
vector<LobbyWorker *> lobbyWorkersVec;
int serviceWorkerCount = 0;
vector<bool> serviceWorkerFree;
vector<ServiceWorker *> serviceWorkersVec;
int beverageWorkerCount = 0;
vector<bool> beverageWorkersFree;
vector<BeverageWorker *> beverageWorkersVec;
int kitchenWorkerCount = 0;
vector<bool> kitchenWorkerFree;
vector<KitchenWorker *> kitchenWorkersVec;

// Ready production
int friesPacked = 0;
int burgersPacked = 0;
int additionsPacked = 0;
int drinksPacked = 0;

// Needed production
int friesOrderCount;
int drinksOrderCount;
int neededMeat;

// Queues of different products orders
queue<Order *> orderQueue;
queue<Order *> packedOrderQueue;
queue<bool> kitchenOrderQueue;

// Time of products preparation start
queue<double> burgerOrderTimes;
queue<double> additionOrderTimes;
queue<double> orderPackExtraditionTimes;
queue<double> friesOrderTimes;
queue<double> drinksOrderTimes;

// Is there an extraditor on the service
bool isExtraditor;

//
// Statistics
//
Stat clientInMCDTime;
Stat wholeClientInMCDTime;

Stat cashRegisterQueueTime;
Stat wholeCashRegisterQueueTime;

Stat kioskQueueTime;
Stat wholeKioskQueueTime;

Stat orderWaitingTime;
Stat wholeOrderWaitingTime;

Stat tableSearchingTime;
Stat wholeTtableSearchingTime;

Stat clientDissatisfaction;
Stat wholeClientDissatisfaction;

Stat burgersTime;
Stat wholeBurgersTime;

Stat friesTime;
Stat wholeFriesTime;

Stat drinksTime;
Stat wholeDrinksTime;

Stat additionsTime;
Stat wholeAdditionsTime;

Stat orderExtraditigTime;
Stat wholeOrderExtraditionTime;

// Client generator behavior
void ClientGenerator::Behavior() {
    (new Client)->Activate(Time);
    Activate(Time + Exponential(clientTime));
}

// Error exiting while reading the experiment
void errorExit(string message) {
    cerr << message << endl;
    exit(1);
}

// Queue of intervals of experiment
queue<ExperimentInterval> experiment;

// The whole time of experiment
double experimentTime = 0;

// Count of parameter in experiment.txt
enum LineMeaning {
    KIOSK_COUNT,
    CASH_REGS,
    TABLE_COUNT,
    DIRTY_EFFECT,
    NO_TABLE_EFFECT,
    FRIES_FRYER_COUNT,
    FRIES_PORTIONS_COUNT,
    RAW_PORTIONS_COUNT,
    MEAT_PORTIONS_COUNT,
    RAW_FRYER_COUNT,
    MEAT_GRILLS_COUNT,

    EXP_TIME,
    CLIENT_FREQ,
    LOBBY,
    SERVICE,
    BEVERAGE,
    KITCHEN,
    MIN_FRIES,
    MIN_MEAT,
    MIN_RAW
};

// Reads the experiment and creates a queue of intervals
void readExperiment() {
    ifstream file("experiment.txt");
    if (!file.is_open())
        errorExit("Error: experiment file cannot be opened");

    string line;
    int lineCounter = KIOSK_COUNT;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        
        size_t lastSpacePos = line.find_last_of(' ');
        string valueString = line.substr(lastSpacePos + 1);
        ExperimentInterval interval;
        switch (lineCounter) {
            case KIOSK_COUNT:
                istringstream(valueString) >> kioskCount;
                if (kioskCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case CASH_REGS:
                istringstream(valueString) >> cashRegisterCount;
                if (cashRegisterCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case TABLE_COUNT:
                istringstream(valueString) >> tableCount;
                if (kioskCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case FRIES_PORTIONS_COUNT:
                istringstream(valueString) >> friesPortionsInSlot;
                if (friesPortionsInSlot <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case MEAT_PORTIONS_COUNT:
                istringstream(valueString) >> meatOnOnePlot;
                if (meatOnOnePlot <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case RAW_PORTIONS_COUNT:
                istringstream(valueString) >> rawPortionsInFryer;
                if (rawPortionsInFryer <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case DIRTY_EFFECT:
                istringstream(valueString) >> dirtyTableInf;
                if (kioskCount < 0)
                    errorExit("Error: dissatisfaction effect cannot be negative!");
                break;
            case NO_TABLE_EFFECT:
                istringstream(valueString) >> noTableInf;
                if (kioskCount < 0)
                    errorExit("Error: dissatisfaction effect cannot be negative!");
                break;
            case FRIES_FRYER_COUNT:
                istringstream(valueString) >> frierSlotsCount;
                if (kioskCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case RAW_FRYER_COUNT:
                istringstream(valueString) >> rawFryerCount;
                if (kioskCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case MEAT_GRILLS_COUNT:
                istringstream(valueString) >> grillPlots;
                if (kioskCount <= 0)
                    errorExit("Error: count must be greater than zero!");
                break;
            case EXP_TIME:
                istringstream(valueString) >> interval.intervalLength;
                experimentTime += interval.intervalLength;
                if (interval.intervalLength <= 0)
                    errorExit("Error: time of experiment interval must be greater than zero!");
                break;
            case CLIENT_FREQ:
                istringstream(valueString) >> interval.clientFrequency;
                if (interval.clientFrequency <= 0)
                    errorExit("Error: client frequency cannot be negative!");
                break;
            case LOBBY:
                istringstream(valueString) >> interval.lobbyWorkers;
                if (interval.lobbyWorkers <= 0)
                    errorExit("Error: count of lobby workers must be greater than zero");
                break;
            case SERVICE:
                istringstream(valueString) >> interval.serviceWorkers;
                if (interval.serviceWorkers < 1 || interval.serviceWorkers > 4)
                    errorExit("Error: count of service workers can be 1, 2, 3 or 4 only!");
                break;
            case BEVERAGE:
                istringstream(valueString) >> interval.beverageWorkers;
                if (interval.beverageWorkers <= 0)
                    errorExit("Error: count of beverage workers must be greater than zero");
                break;
            case KITCHEN:
                istringstream(valueString) >> interval.kitchenWorkers;
                if (interval.kitchenWorkers < 2 || interval.kitchenWorkers > 4)
                    errorExit("Error: count of kitchen workers can be 2, 3 or 4 only!");
                break;
            case MIN_FRIES:
                istringstream(valueString) >> interval.friesCount;
                if (interval.friesCount <= 0)
                    errorExit("Error: normal count of fries must be reater than zero");
                break;
            case MIN_MEAT:
                istringstream(valueString) >> interval.meatCount;
                if (interval.meatCount <= 0)
                    errorExit("Error: normal count of meat must be reater than zero");
                break;
            case MIN_RAW:
                istringstream(valueString) >> interval.rawCount;
                if (interval.rawCount <= 0)
                    errorExit("Error: normal count of raw must be reater than zero");
                experiment.push(interval);
                lineCounter = MEAT_GRILLS_COUNT;
                break;
            default:
                break;
        }
        lineCounter++;
    }

    if (experiment.size() == 0)
        errorExit("Error: at least one experiment interval must be described in experiment.txt");
}