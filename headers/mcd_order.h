/******************************************************************************
 *                                  MCD
 *                              mcd_order.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: Declaration of order class
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#ifndef MCD_ORDER
#define MCD_ORDER

#include "simlib.h"

class Client;

class Order : public Process {
public:
    int burgers;        // Count of burgers
    int additions;      // Count of additions
    int fries;          // Count of fries
    int drinks;         // Count of drinks
    bool pack;          // Should the order be packed
    Client * client;    // Pointer to the client object

    Order(int b, int a, int f, int d, bool p, Client * c);
    void Behavior(); 
};

#endif 