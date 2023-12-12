#ifndef MCD_SERVICE
#define MCD_SERVICE

#include "simlib.h"

class ServiceWorker : public Process {
public:
    void extraditeOrder();
    void tryToPackOrder();
};

class Packer : public ServiceWorker {
    void Behavior();
};

class Extraditor : public ServiceWorker {
    void Behavior();
};

#endif