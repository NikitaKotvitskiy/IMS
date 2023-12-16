/******************************************************************************
 *                                  MCD
 *                              mcd_params.h
 * 
 *      Authors: Nikita Kotvitskiy  
 *      Purpose: This header contains the average and constant parameters of the
 *               restaurant, like the count of different facilities, chances, time
 *               characteristics and so on 
 * 
 *                        Last change: 16.12.2023
 *****************************************************************************/

#ifndef MCD_PARAMS_H
#define MCD_PARAMS_H
#include <queue>
#include <vector>

using namespace std;

struct NormalParams {
    double center;
    double scattering;
};

// Average gaps between clients
extern double clientTime;

// MCD devices count
extern int cashRegisterCount;
extern int kioskCount;
extern int tableCount;
const int drinkDistributorCount = 5;

//
// Time parameters
//

// Client processes times
const NormalParams whereToOrderTime         = {.center = 0.16, .scattering = 0.035};
const NormalParams whichKiosk               = {.center = 0.08, .scattering = 0.015};
const NormalParams whichCashRegister        = {.center = 0.08, .scattering = 0.015};
const NormalParams whatKiosk                = {.center = 3.50, .scattering = 0.060};
const NormalParams whatCashRegister         = {.center = 2.00, .scattering = 0.040};
const NormalParams payTime                  = {.center = 0.16, .scattering = 0.032};
const NormalParams pickOrderTime            = {.center = 0.08, .scattering = 0.010};
const NormalParams searchTableTime          = {.center = 0.05, .scattering = 0.010};
const NormalParams eatOneItemTime           = {.center = 2.00, .scattering = 0.450};

const NormalParams assessTime               = {.center = 0.02, .scattering = 0.004};

// Lobby workers times
const NormalParams tableCleaningTime        = {.center = 0.25, .scattering = 0.050};

// Service workers times
const NormalParams packAnOrderTime          = {.center = 1.50, .scattering = 0.290};
const NormalParams orderExtraditionTime     = {.center = 0.16, .scattering = 0.030};

// Fries fryer times
const NormalParams friesPackingTime         = {.center = 0.08, .scattering = 0.010};
const NormalParams friesTransferTime        = {.center = 0.10, .scattering = 0.010};
const NormalParams friesFryingPrepareTime   = {.center = 0.13, .scattering = 0.010};
const double friesPreparingTime             = 2;

// Beverage worker times
const NormalParams drinkPreparingTime       = {.center = 0.05, .scattering = 0.010};
const NormalParams drinkPackingTime         = {.center = 0.05, .scattering = 0.010};
const double drinkFillingTime               = 0.10;

// Kitchen workers times
const NormalParams bunsTransferTime         = {.center = 0.08, .scattering = 0.015};
const NormalParams bunsToToasterTime        = {.center = 0.06, .scattering = 0.005};
const NormalParams singleBurgerFillingTime  = {.center = 0.50, .scattering = 0.050};
const NormalParams doubleBurgerFillingTime  = {.center = 0.75, .scattering = 0.075};
const NormalParams singleBurgerPackingTime  = {.center = 0.11, .scattering = 0.010};
const NormalParams doubleBurgerPackingTime  = {.center = 0.17, .scattering = 0.013};
const NormalParams addPackingTime           = {.center = 0.36, .scattering = 0.050};
const NormalParams addOrderingTime          = {.center = 0.05, .scattering = 0.009};
const NormalParams rawToTrayTime            = {.center = 0.21, .scattering = 0.030};
const NormalParams rawToFryerTime           = {.center = 0.30, .scattering = 0.050};
const NormalParams meatPreparingStartTime   = {.center = 0.55, .scattering = 0.060};
const NormalParams meatTransferTime         = {.center = 0.48, .scattering = 0.054};
const double meatTime                       = 1.25;
const double rawTime                        = 3.90;
const double bunTime                        = 0.36;

//
// Client addition parameters
//

// Dissatisfaction influences
extern int dirtyTableInf;
extern int noTableInf;

// Client chances
const double kioskChance                = 0.50;
const double smallOrderChance           = 0.7;
const double middleOrderChance          = 0.25;
const double payFailed                  = 0.01;
const double eatInMCD                   = 0.50;
const double denyTable                  = 0.05;
const double cancelMeal                 = 0.05;
const double packRemains                = 0.70;
const double badClient                  = 0.50;

// Count of different produces in orders
const int smallOrderPossibleBurgerCountMin      = 0;
const int smallOrderPossibleBurgerCountMax      = 2;
const int smallOrderPossibleAdditionCountMin    = 0;
const int smallOrderPossibleAdditionCountMax    = 2;
const int smallOrderPossibleFriesCountMin       = 0;
const int smallOrderPossibleFriesCountMax       = 2;
const int smallOrderPossibleDrinkCountMin       = 0;
const int smallOrderPossibleDrinkCountMax       = 2;

const int middleOrderPossibleBurgerCountMin     = 1;
const int middleOrderPossibleBurgerCountMax     = 3;
const int middleOrderPossibleAdditionCountMin   = 1;
const int middleOrderPossibleAdditionCountMax   = 3;
const int middleOrderPossibleFriesCountMin      = 1;
const int middleOrderPossibleFriesCountMax      = 2;
const int middleOrderPossibleDrinkCountMin      = 1;
const int middleOrderPossibleDrinkCountMax      = 3;

const int bigOrderPossibleBurgerCountMin        = 2;
const int bigOrderPossibleBurgerCountMax        = 10;
const int bigOrderPossibleAdditionCountMin      = 2;
const int bigOrderPossibleAdditionCountMax      = 10;
const int bigOrderPossibleFriesCountMin         = 2;
const int bigOrderPossibleFriesCountMax         = 5;
const int bigOrderPossibleDrinkCountMin         = 2;
const int bigOrderPossibleDrinkCountMax         = 6;



//
// Workers addition parameters
//

// Frier parameters 
extern int minimalFries;
extern int frierSlotsCount;
extern int friesPortionsInSlot;
extern int rawPortionsInFryer;

// Kitchen paraneters
extern int rawMinimum;
extern int rawFryerCount;
extern int grillPlots;
extern int meatMinimum;
extern int meatOnOnePlot;

#endif