/******************************************************************************
 *                                  MCD
 *                             mcd_client.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of client class
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#ifndef MCD_CLIENT
#define MCD_CLIENT

#include "simlib.h"

class Order;

class Client : public Process {
public:
    int clientNumber;           // ID of client (it can be used while debugging)
    double arriveTime;          // Time of arrival in the restaurant
    bool orderInCashRegister;   // Does the client want to make his order in a cash register
    int burgers = 0;            // Count of burgers in client's order
    int additions = 0;          // Count of additions in client's order
    int fries = 0;              // Count of frues in client's order
    int drinks = 0;             // Count of frinks in client's order
    int dissatisfaction = 0;    // Dissatisfaction of client
    bool packOrder;             // Does the client want to eat in the restaurant
    bool tableFound;            // Has the client found a table
    Order * order;              // Pointer to client's order
    int chosenTable;            // Number of client's table
    double startWaitTime;       // When client has started to wait

    void saveStats();
    void makeAnOrder();
    int chooseFacility();
    void Behavior();
};

#endif