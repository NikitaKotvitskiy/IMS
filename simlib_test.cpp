#include "simlib.h"
#include "mcd.h"
#include <iostream>

using namespace std;

int main() {
    Init(0, 100);
    initMCD();
    Run();
    cout << clientsLeft << endl;
    return 0;
}