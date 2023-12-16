/******************************************************************************
 *                                  MCD
 *                           mcd_srvice_worker.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of service workers' classes
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#ifndef MCD_SERVICE
#define MCD_SERVICE

#include "simlib.h"

class ServiceWorker : public Process {
public:
    int id;
    
    void extraditeOrder();
    void tryToPackOrder();
    void processClientRequest();
};

// Class of packer, who packs the orders
class Packer : public ServiceWorker {
    void Behavior();
};

// Class of extraditor, who processes client requests and extradite the orders
class Extraditor : public ServiceWorker {
    void Behavior();
};

#endif