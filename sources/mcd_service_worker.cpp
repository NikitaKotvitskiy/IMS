#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

void ServiceWorker::extraditeOrder() {
    if (SERVICE_DEBUG_MODE) cout << Time << ": serivce worker is extraditing an order" << endl;
    Order * order = packedOrderQueue.front();
    packedOrderQueue.pop();
    Wait(Normal(orderExtraditionTime.center, orderExtraditionTime.scattering));
    order->client->Activate(Time);
    order->Cancel();
}

void ServiceWorker::tryToPackOrder() {
    if (!orderQueue.empty()) {
        Order * order = orderQueue.front();
        if (order->burgers != 0 && burgersPacked >= order->burgers || order->burgers == 0)
            if (order->additions != 0 && additionsPacked >= order->additions || order->additions == 0)
                if (order->fries != 0 && friesPacked >= order->fries || order->fries == 0)
                    if (order->drinks != 0 && drinksPacked >= order->drinks || order->drinks == 0) {
                        if (SERVICE_DEBUG_MODE) cout << Time << ": serivce worker is packing an order" << endl;
                        burgersPacked -= order->burgers;
                        additionsPacked -= order->additions;
                        friesPacked -= order->fries;
                        drinksPacked -= order->drinks;
                        orderQueue.pop();
                        Wait(Normal(packAnOrderTime.center, packAnOrderTime.scattering));
                        packedOrderQueue.push(order);
                    }
    }
}

void ServiceWorker::processClientRequest() {
    if (SERVICE_DEBUG_MODE) cout << Time << ": serivce worker has accepted client request and is packing his order" << endl;

    Client * client = extradition.front();
    extradition.pop();
    Wait(Normal(packAnOrderTime.center, packAnOrderTime.scattering));
    client->Activate(Time);
}

void Extraditor::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (!extradition.empty()) {
            processClientRequest();
            continue;
        }

        if (!packedOrderQueue.empty())
            extraditeOrder();
    }
}

void Packer::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (!isExtraditor && !extradition.empty()) {
            processClientRequest();
            continue;
        }

        if (!isExtraditor && !packedOrderQueue.empty()) {
            extraditeOrder();
            continue;
        }

        tryToPackOrder();
    }
}