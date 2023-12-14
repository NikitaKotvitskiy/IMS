#include "simlib.h"
#include "mcd.h"
#include <queue>

using namespace std;

int clientCounter = 0;

Facility cashRegisters[cashRegisterCount];
Facility kiosks[kioskCount];
Queue extradition;
Table tables[tableCount];

int friesPacked = 0;
int burgersPacked = 0;
int additionsPacked = 0;
int drinksPacked = 0;

int friesOrderCount = 0;
int drinksOrderCount = 0;

queue<Order *> orderQueue;
queue<Order *> packedOrderQueue;
queue<bool> kitchenOrderQueue;
int neededMeat = 0;

bool isExtraditor;

void initMCD() {
    clientInMCDTime.SetName("Customer time spent in MCD");
    clientDissatisfaction.SetName("Customer dissatisfaction");
    cashRegisterQueueTime.SetName("Time in cash register queue");
    kioskQueueTime.SetName("Time in kiosk queue");
    orderWaitingTime.SetName("Time of waiting for an order");
    tableSearchingTime.SetName("Time of table searching");

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

    for (int i = 0; i < beverageWorkerCount; i++)
        (new BeverageWorker)->Activate(Time);
    
    switch (kitchenWorkerCount) {
        case 2:
            (new SingleFinisher)->Activate(Time);
            (new Iniciator)->Activate(Time);
            break;
        case 3:
            (new SingleFinisher)->Activate(Time);
            (new Assambler)->Activate(Time);
            (new Iniciator)->Activate(Time);
            break;
        case 4:
            (new FinisherFryer)->Activate(Time);
            (new FinisherPacker)->Activate(Time);
            (new Assambler)->Activate(Time);
            (new Iniciator)->Activate(Time);
            break;
    }

    (new Griller)->Activate(Time);
}

void ClientGenerator::Behavior() {
    (new Client)->Activate(Time);
    Activate(Time + Exponential(clientTime));
}

// Statistic
Stat clientInMCDTime;
Stat cashRegisterQueueTime;
Stat kioskQueueTime;
Stat orderWaitingTime;
Stat clientDissatisfaction;
Stat tableSearchingTime;