#ifndef MCD_CLIENT
#define MCD_CLIENT

#include "simlib.h"

class Order;

class Client : public Process {
public:
    bool orderInCashRegister;
    int burgers = 0;
    int additions = 0;
    int fries = 0;
    int drinks = 0;
    int dissatisfaction = 0;
    bool packOrder;
    bool tableFound;
    Order * order;
    int chosenTable;
    double startWaitTime;

    int clientNumber; // TMP

    enum OrderState {
	    ORDER_START,
	    ORDER_BURGERS,
	    ORDER_ADDITIONS,
	    ORDER_FRIES,
	    ORDER_DRINKS,
	    ORDER_END
    };

    OrderState state = ORDER_START;

    void makeAnOrder();
    int chooseFacility();
    void Behavior();
};

#endif