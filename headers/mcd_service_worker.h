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

class Packer : public ServiceWorker {
    void Behavior();
};

class Extraditor : public ServiceWorker {
    void Behavior();
};

#endif