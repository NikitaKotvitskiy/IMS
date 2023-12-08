#include "simlib.h"
#include <iostream>

using namespace std;

const int rawTime = 90;
int rawReady = 0;

// deklarace obslužných linek

// deklarace tříd popisujících procesy a události
class Raw : public Process {
    void Behavior() {
        Wait(rawTime);
        cout << Time << ": Raw is ready" << endl;
        rawReady++;
    }
};

class RawMaker : public Event {
    void Behavior() {
        (new Raw)->Activate();
        double nextTime = Random() * 100;
        cout << Time << ": New raw cooking starts..." << endl;
        cout << "Next raw will start to cook after " << nextTime << endl;
        Activate(Time + nextTime);
    }
};

// popis simulačního experimentu ve funkci main()
int main() {
    Init(0, 1000);
    (new RawMaker)->Activate();
    Run();
    cout << rawReady << endl;
    return 0;
}