#include "simlib.h"
#include "mcd.h"
#include <queue>

using namespace std;

int clientsLeft = 0;
int clientCounter = 0;

Facility cashRegisters[cashRegisterCount];
Facility kiosks[kioskCount];
Queue extradition;
Table tables[tableCount];

int friesPacked = 0;
int burgersPacked = 10000;
int additionsPacked = 10000;
int drinksPacked = 10000;

int friesOrderCount = 0;
int friesReady = 0;
int friesPreparing = 0;
int friesFinished = 0;

queue<Order *> orderQueue;
queue<Order *> packedOrderQueue;

bool isExtraditor;

void initMCD() {
    (new ClientGenerator)->Activate(Time);

    for (int i = 0; i < lobbyWorkerCount; i++)
        (new LobbyWorker)->Activate(Time);
    
    switch (serviceWorkerCount) {
        case 1:
            isExtraditor = false;
            (new Packer)->Activate(Time);
            break;
        case 2:
            isExtraditor = true;
            (new Packer)->Activate(Time);
            (new Extraditor)->Activate(Time);
            break;
        case 3:
            isExtraditor = true;
            (new Packer)->Activate(Time);
            (new Packer)->Activate(Time);
            (new Extraditor)->Activate(Time);
            break;
        case 4:
            isExtraditor = true;
            (new Packer)->Activate(Time);
            (new Packer)->Activate(Time);
            (new Extraditor)->Activate(Time);
            (new Extraditor)->Activate(Time);
            break;
    }

    (new Frier)->Activate(Time);
}

void ClientGenerator::Behavior() {
    (new Client)->Activate(Time);
    cout << Time << ": client " << clientCounter << " has entered the MCD" << endl;
    Activate(Time + Exponential(10));
}