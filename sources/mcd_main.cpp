#include "simlib.h"
#include "../headers/mcd.h"
#include <iostream>

using namespace std;

void printWholeStat() {
    wholeClientInMCDTime.SetName("Whole stat: Customer time spent in MCD");
    wholeClientDissatisfaction.SetName("Whole stat: Customer dissatisfaction");
    wholeCashRegisterQueueTime.SetName("Whole stat: Time in cash register queue");
    wholeKioskQueueTime.SetName("Whole stat: Time in kiosk queue");
    wholeOrderWaitingTime.SetName("Whole stat: Time of waiting for an order");
    wholeTtableSearchingTime.SetName("Whole stat: Time of table searching");
    wholeBurgersTime.SetName("Whole stat: Time of burger preparing");
    wholeAdditionsTime.SetName("Whole stat: Time of addition preparing");
    wholeFriesTime.SetName("Whole stat: Time of fries preparing");
    wholeDrinksTime.SetName("Whole stat: Time of drink preparing");
    wholeOrderExtraditionTime.SetName("Whole stat: Time of order extraditing");

    wholeClientInMCDTime.Output();
    wholeClientDissatisfaction.Output();
    wholeCashRegisterQueueTime.Output();
    wholeKioskQueueTime.Output();
    wholeOrderWaitingTime.Output();
    wholeTtableSearchingTime.Output();
    wholeBurgersTime.Output();
    wholeAdditionsTime.Output();
    wholeFriesTime.Output();
    wholeDrinksTime.Output();
    wholeOrderExtraditionTime.Output();
}

int main() {
    readExperiment();
    SetOutput("output.txt");
    Init(0, experimentTime);
    Experiment * exp = new Experiment;
    exp->Activate();
    Run();
    printStats();
    printWholeStat();
    return 0;
}