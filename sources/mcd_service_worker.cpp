/******************************************************************************
 *                                  MCD
 *                          mcd_service_worker.cpp
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Definition of service workers' behavior
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#include <iostream>
#include "simlib.h"
#include "../headers/mcd.h"

// Process of order extraditing
void ServiceWorker::extraditeOrder() {
    if (SERVICE_DEBUG_MODE) cout << Time << ": serivce worker is extraditing an order" << endl;
    Order * order = packedOrderQueue.front();
    packedOrderQueue.pop();
    Wait(Normal(orderExtraditionTime.center, orderExtraditionTime.scattering));
    order->client->Activate(Time);
    order->Cancel();
    orderExtraditigTime(Time - orderPackExtraditionTimes.front());
    wholeOrderExtraditionTime(Time - orderPackExtraditionTimes.front());
    orderPackExtraditionTimes.pop();
}

// Process of order packing
void ServiceWorker::tryToPackOrder() {
    // If all elements of the order are ready, worker will start to pack ther order
    if (!orderQueue.empty()) {
        Order * order = orderQueue.front();
        if (order->burgers != 0 && burgersPacked >= order->burgers || order->burgers == 0)
            if (order->additions != 0 && additionsPacked >= order->additions || order->additions == 0)
                if (order->fries != 0 && friesPacked >= order->fries || order->fries == 0)
                    if (order->drinks != 0 && drinksPacked >= order->drinks || order->drinks == 0) {
                        orderPackExtraditionTimes.push(Time);
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

// Process of client request processing
void ServiceWorker::processClientRequest() {
    if (SERVICE_DEBUG_MODE) cout << Time << ": serivce worker has accepted client request and is packing his order" << endl;

    Client * client = extradition.front();
    extradition.pop();
    Wait(Normal(packAnOrderTime.center, packAnOrderTime.scattering));
    client->Activate(Time);
}

// Behavior of extraditor
void Extraditor::Behavior() {
    while(true) {
        serviceWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        serviceWorkerFree[id] = false;

        // If there is a client who wants his order packed, extraditor will help him
        if (!extradition.empty()) {
            processClientRequest();
            continue;
        }

        // If some order is ready to be extradited, extraditor will extradite it
        if (!packedOrderQueue.empty())
            extraditeOrder();
    }
}


// Behavior of packer
void Packer::Behavior() {
    while(true) {
        serviceWorkerFree[id] = true;
        Wait(Normal(assessTime.center, assessTime.scattering));
        serviceWorkerFree[id] = false;

        // If there isn't extraditor but there is a client who wants his order packed, packer will help him
        if (!isExtraditor && !extradition.empty()) {
            processClientRequest();
            continue;
        }

        // If there isn't extraditor but there is a ready order, packer will extradite it
        if (!isExtraditor && !packedOrderQueue.empty()) {
            extraditeOrder();
            continue;
        }

        // If there is some order, packer will check if he has all it's elements and pack it
        tryToPackOrder();
    }
}