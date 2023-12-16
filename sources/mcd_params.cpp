#include <iostream>
#include <list>
#include "simlib.h"
#include "../headers/mcd_params.h"
#include "../headers/mcd.h"

using namespace std;

double clientTime;

int kioskCount;
int cashRegisterCount;
int tableCount;
int dirtyTableInf;
int noTableInf;
int frierSlotsCount;
int rawFryerCount;
int grillPlots;

int minimalFries;
int friesPortionsInSlot;

int rawMinimum;
int meatMinimum;
int meatOnOnePlot;
int rawPortionsInFryer;;

int intervalsCount = 0;

void setStatNames(int intervalNum) {
    clientInMCDTime.Clear();
    cashRegisterQueueTime.Clear();
    kioskQueueTime.Clear();
    orderWaitingTime.Clear();
    tableSearchingTime.Clear();
    clientDissatisfaction.Clear();
    burgersTime.Clear();
    additionsTime.Clear();
    friesTime.Clear();
    drinksTime.Clear();
    orderExtraditigTime.Clear();

    clientInMCDTime.SetName("Interval " + to_string(intervalNum) + ": Customer time spent in MCD");
    clientDissatisfaction.SetName("Interval " + to_string(intervalNum) + ": Customer dissatisfaction");
    cashRegisterQueueTime.SetName("Interval " + to_string(intervalNum) + ": Time in cash register queue");
    kioskQueueTime.SetName("Interval " + to_string(intervalNum) + ": Time in kiosk queue");
    orderWaitingTime.SetName("Interval " + to_string(intervalNum) + ": Time of waiting for an order");
    tableSearchingTime.SetName("Interval " + to_string(intervalNum) + ": Time of table searching");
    burgersTime.SetName("Interval " + to_string(intervalNum) + ": Time of burger preparing");
    additionsTime.SetName("Interval " + to_string(intervalNum) + ": Time of addition preparing");
    friesTime.SetName("Interval " + to_string(intervalNum) + ": Time of fries preparing");
    drinksTime.SetName("Interval " + to_string(intervalNum) + ": Time of drink preparing");
    orderExtraditigTime.SetName("Interval " + to_string(intervalNum) + ": Time of order extraditing");
}

void printStats() {
    clientInMCDTime.Output();
    cashRegisterQueueTime.Output();
    kioskQueueTime.Output();
    orderWaitingTime.Output();
    tableSearchingTime.Output();
    clientDissatisfaction.Output();
    burgersTime.Output();
    additionsTime.Output();
    friesTime.Output();
    drinksTime.Output();
    orderExtraditigTime.Output();
}

void setConstantParams() {
    for (int i = 0; i < cashRegisterCount; i++)
        cashRegisters.push_back(new Facility());
    for (int i = 0; i < kioskCount; i++)
        kiosks.push_back(new Facility());
    for (int i = 0; i < tableCount; i++) {
        Table newTable;
        tables.push_back(newTable);
    }
}

void Experiment::Behavior() {
    intervalsCount = experiment.size();
    setConstantParams();

    (new Griller)->Activate();
    (new Frier)->Activate();
    (new ClientGenerator)->Activate();

    Seize(*(cashRegisters)[0]);
    Release(*(cashRegisters)[0]);

    int experimentCounter = 1;
    setStatNames(experimentCounter);

    while (!experiment.empty()) {
        ExperimentInterval currentInterval = experiment.front();
        experiment.pop();

        int difference = currentInterval.lobbyWorkers - lobbyWorkerCount;
        if (difference > 0)
            for (int i = 0; i < difference; i++) {
                LobbyWorker * worker = new LobbyWorker();
                worker->id = lobbyWorkerCount++;
                lobbyWorkersVec.push_back(worker);
                lobbyWorkerFree.push_back(true);
                worker->Activate();
            }
        else if (difference < 0) {
            for (int i = 0; i < abs(difference); i++) {
                while (!lobbyWorkerFree[lobbyWorkerCount - 1])
                    Wait(0.005);
                lobbyWorkersVec[--lobbyWorkerCount]->Cancel();
                lobbyWorkersVec.pop_back();
                lobbyWorkerFree.pop_back();
            }
        }

        difference = currentInterval.beverageWorkers - beverageWorkerCount;
        if (difference > 0)
            for (int i = 0; i < difference; i++) {
                BeverageWorker * worker = new BeverageWorker();
                worker->id = beverageWorkerCount++;
                beverageWorkersVec.push_back(worker);
                beverageWorkersFree.push_back(true);
                worker->Activate();
            }
        else if (difference < 0) {
            for (int i = 0; i < abs(difference); i++) {
                while (!beverageWorkersFree[beverageWorkerCount - 1])
                    Wait(0.005);
                beverageWorkerCount--;
                beverageWorkersVec[--beverageWorkerCount]->Cancel();
                beverageWorkersVec.pop_back();
                beverageWorkersFree.pop_back();
            }
        }

        difference = currentInterval.serviceWorkers - serviceWorkerCount;
        if (difference != 0) {
            for (int i = 0; i < serviceWorkerCount; i++) {
                while (!serviceWorkerFree[i]) 
                    Wait(0.005);
                serviceWorkersVec[i]->Cancel();
            }
            serviceWorkersVec.clear();
            serviceWorkerFree.clear();
            serviceWorkerCount = 0;

            switch (currentInterval.serviceWorkers) {
                case 1: {
                    Packer * worker = new Packer();
                    worker->id = serviceWorkerCount++;
                    serviceWorkersVec.push_back(worker);
                    break;
                }
                case 2: {
                    Packer * worker1 = new Packer();
                    Extraditor * worker2 = new Extraditor();
                    worker1->id = serviceWorkerCount++;
                    worker2->id = serviceWorkerCount++;
                    serviceWorkersVec.push_back(worker1);
                    serviceWorkersVec.push_back(worker2);
                    break;
                }
                case 3: {
                    Packer * worker1 = new Packer();
                    Packer * worker2 = new Packer();
                    Extraditor * worker3 = new Extraditor();
                    worker1->id = serviceWorkerCount++;
                    worker2->id = serviceWorkerCount++;
                    worker3->id = serviceWorkerCount++;
                    serviceWorkersVec.push_back(worker1);
                    serviceWorkersVec.push_back(worker2);
                    serviceWorkersVec.push_back(worker3);
                    break;
                }
                case 4: {
                    Packer * worker1 = new Packer();
                    Packer * worker2 = new Packer();
                    Extraditor * worker3 = new Extraditor();
                    Extraditor * worker4 = new Extraditor();
                    worker1->id = serviceWorkerCount++;
                    worker2->id = serviceWorkerCount++;
                    worker3->id = serviceWorkerCount++;
                    worker4->id = serviceWorkerCount++;
                    serviceWorkersVec.push_back(worker1);
                    serviceWorkersVec.push_back(worker2);
                    serviceWorkersVec.push_back(worker3);
                    serviceWorkersVec.push_back(worker4);
                    break;
                }
            }

            for (int i = 0; i < serviceWorkerCount; i++) {
                serviceWorkerFree.push_back(true);
                serviceWorkersVec[i]->Activate(Time);
            }
        }

        difference = currentInterval.kitchenWorkers - kitchenWorkerCount;
        if (difference != 0) {
            for (int i = 0; i < kitchenWorkerCount; i++) {
                while (!kitchenWorkerFree[i]) 
                    Wait(0.005);
                kitchenWorkersVec[i]->Cancel();
            }
            kitchenWorkersVec.clear();
            kitchenWorkerFree.clear();
            kitchenWorkerCount = 0;

            switch (currentInterval.kitchenWorkers) {
                case 2: {
                    SingleFinisher * worker1 = new SingleFinisher();
                    Iniciator * worker2 = new Iniciator();
                    worker1->id = kitchenWorkerCount++;
                    worker2->id = kitchenWorkerCount++;
                    kitchenWorkersVec.push_back(worker1);
                    kitchenWorkersVec.push_back(worker2);
                    break;
                }
                case 3: {
                    SingleFinisher * worker1 = new SingleFinisher();
                    Assambler * worker2 = new Assambler();
                    Iniciator * worker3 = new Iniciator();
                    worker1->id = kitchenWorkerCount++;
                    worker2->id = kitchenWorkerCount++;
                    worker3->id = kitchenWorkerCount++;
                    kitchenWorkersVec.push_back(worker1);
                    kitchenWorkersVec.push_back(worker2);
                    kitchenWorkersVec.push_back(worker3);
                    break;
                }
                case 4: {
                    FinisherFryer * worker1 = new FinisherFryer();
                    FinisherPacker * worker2 = new FinisherPacker();
                    Assambler * worker3 = new Assambler();
                    Iniciator * worker4 = new Iniciator();
                    worker1->id = kitchenWorkerCount++;
                    worker2->id = kitchenWorkerCount++;
                    worker3->id = kitchenWorkerCount++;
                    worker4->id = kitchenWorkerCount++;
                    kitchenWorkersVec.push_back(worker1);
                    kitchenWorkersVec.push_back(worker2);
                    kitchenWorkersVec.push_back(worker3);
                    kitchenWorkersVec.push_back(worker4);
                    break;
                }
            }

            for (int i = 0; i < kitchenWorkerCount; i++) {
                kitchenWorkerFree.push_back(true);
                kitchenWorkersVec[i]->Activate(Time);
            }
        }

        clientTime = currentInterval.clientFrequency;
        minimalFries = currentInterval.friesCount;
        meatMinimum = currentInterval.meatCount;
        rawMinimum = currentInterval.rawCount;

        Wait(currentInterval.intervalLength);
        if (experimentCounter >= intervalsCount)
            break;
        printStats();
        setStatNames(++experimentCounter);
    }
}