#ifndef MCD_PARAMS_H
#define MCD_PARAMS_H

struct NormalParams {
    double center;
    double scattering;
};

// Average gaps between clients
const double clientTime = 1;

// MCD parameters
const int cashRegisterCount = 2;
const int kioskCount = 5;
const int tableCount = 10;

// Clients params
const int dirtyTableInf = 5;
const int noTableInf = 5;

// Workers params
const int lobbyWorkerCount = 1;
const int serviceWorkerCount = 2;
const int beverageWorkerCount = 1;
const int kitchenWorkerCount = 4;

//
// Client parameters
//

// Client times
const NormalParams whereToOrderTime     = {.center = 0.16, .scattering = 0.035};
const NormalParams whichKiosk           = {.center = 0.08, .scattering = 0.015};
const NormalParams whichCashRegister    = {.center = 0.08, .scattering = 0.015};
const NormalParams whatKiosk            = {.center = 0.60, .scattering = 0.120};
const NormalParams whatCashRegister     = {.center = 0.13, .scattering = 0.025};
const NormalParams payTime              = {.center = 0.16, .scattering = 0.032};
const NormalParams pickOrderTime        = {.center = 0.08, .scattering = 0.010};
const NormalParams searchTableTime      = {.center = 0.05, .scattering = 0.010};
const NormalParams eatOneItemTime       = {.center = 2.00, .scattering = 0.450};

// Client chances
const double kioskChance                = 0.60;
const double orderBurgerChance          = 0.80;
const double orderAdditionChance        = 0.65;
const double orderFriesChance           = 0.60;
const double orderDrinkChance           = 0.75;
const double orderAnotherBurgerChance   = 0.50;
const double orderAnotherAdditionChance = 0.45;
const double orderAnotherFriesChance    = 0.45;
const double orderAnotherDrinkChance    = 0.20;
const double payFailed                  = 0.01;
const double eatInMCD                   = 0.50;
const double denyTable                  = 0.05;
const double cancelMeal                 = 0.05;
const double packRemains                = 0.70;
const double badClient                  = 0.50;

//
// Workers parameters
//

const NormalParams assessTime = {.center = 0.02, .scattering = 0.004};

// Lobby workers parameters
const NormalParams tableCleaningTime = {.center = 0.25, .scattering = 0.05};

// Service workers parameters
const NormalParams packAnOrderTime      = {.center = 0.50, .scattering = 0.10};
const NormalParams orderExtraditionTime = {.center = 0.16, .scattering = 0.03};

// Frier parameters 
const int minimalFries          = 18;
const int frierSlotsCount       = 4;
const int friesPortionsInSlot   = 6;

const NormalParams friesPackingTime         = {.center = 0.08, .scattering = 0.01};
const NormalParams friesTransferTime        = {.center = 0.10, .scattering = 0.01};
const NormalParams friesFryingPrepareTime   = {.center = 0.13, .scattering = 0.01};
const double friesPreparingTime             = 2;

// Beverage parameters
const int drinkDistributorCount = 5;

const NormalParams drinkPreparingTime       = {.center = 0.05, .scattering = 0.01};
const NormalParams drinkPackingTime         = {.center = 0.05, .scattering = 0.01};
const double drinkFillingTime               = 0.10;

// Kitchen paraneters
const int rawMinimum    = 10;
const int rawFryerCount = 6;
const int grillPlots    = 4;
const int meatMinimum   = 12;
const int meatOnOnePlot = 6;

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

#endif