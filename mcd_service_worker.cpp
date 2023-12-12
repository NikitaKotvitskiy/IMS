#include <iostream>
#include "simlib.h"
#include "mcd.h"

void ServiceWorker::extraditeOrder() {
    cout << Time << ": Service worker is extradition the order" << endl;
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
                        cout << Time << ": Service worker is packing the order" << endl;
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

void Extraditor::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));

        if (!packedOrderQueue.empty())
            extraditeOrder();
    }
}

void Packer::Behavior() {
    while(true) {
        Wait(Normal(assessTime.center, assessTime.scattering));
        if (!isExtraditor && !packedOrderQueue.empty()) {
            extraditeOrder();
            continue;
        }

        tryToPackOrder();
    }
}