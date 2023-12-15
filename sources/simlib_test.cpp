#include "simlib.h"
#include "../headers/mcd.h"
#include <iostream>

using namespace std;

int main() {
    readExperiment();
    SetOutput("stats");
    Init(0, experimentTime);
    Experiment * exp = new Experiment;
    exp->Activate();
    Run();
    printStats();
    return 0;
}