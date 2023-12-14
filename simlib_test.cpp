#include "simlib.h"
#include "mcd.h"
#include <iostream>

using namespace std;

int main() {
    Init(0, 300);
    initMCD();
    Run();
    SetOutput("Stats");
    clientInMCDTime.Output();
    clientDissatisfaction.Output();
    cashRegisterQueueTime.Output();
    kioskQueueTime.Output();
    orderWaitingTime.Output();
    tableSearchingTime.Output();
    burgersTime.Output();
    additionsTime.Output();
    friesTime.Output();
    drinksTime.Output();
    return 0;
}