#ifndef MCD_ORDER
#define MCD_ORDER

#include "simlib.h"

class Client;

class Order : public Process {
public:
    int burgers;
    int additions;
    int fries;
    int drinks;
    bool pack;
    Client * client;

    Order(int b, int a, int f, int d, bool p, Client * c);
    void Behavior(); 
};

#endif 