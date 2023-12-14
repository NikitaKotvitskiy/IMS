#ifndef MCD_PARAMS_H
#define MCD_PARAMS_H

struct NormalParams {
    double center;
    double scattering;
};

// Average gaps between clients
const double clientTime = 1.5;

// MCD parameters
const int cashRegisterCount = 2;
const int kioskCount = 5;
const int tableCount = 10;

// Clients params
const int dirtyTableInf = 5;
const int noTableInf = 5;

// Workers params
const int lobbyWorkerCount = 1;
const int serviceWorkerCount = 1;
const int beverageWorkerCount = 1;
const int kitchenWorkerCount = 3;

//
// Client parameters
//

// Client times
const NormalParams whereToOrderTime = {.center = 0.16, .scattering = 0.04};
const NormalParams whichKiosk = {.center = 0.16, .scattering = 0.04};
const NormalParams whichCashRegister = {.center = 0.16, .scattering = 0.04};
const NormalParams whatKiosk = {.center = 0.6, .scattering = 0.15};
const NormalParams whatCashRegister = {.center = 1, .scattering = 0.2};
const NormalParams payTime = {.center = 0.1, .scattering = 0.01};
const NormalParams pickOrderTime = {.center = 0.1, .scattering = 0.01};
const NormalParams searchTableTime = {.center = 0.16, .scattering = 0.05};
const NormalParams eatOneItemTime = {.center = 1.5, .scattering = 0.04};

// Client chances
const double kioskChance = 0.5;

const double orderBurgerChance = 0.6;
const double orderAdditionChance = 0.5;
const double orderFriesChance = 0.5;
const double orderDrinkChance = 0.5;
const double orderAnotherBurgerChance = 0.3;
const double orderAnotherAdditionChance = 0.25;
const double orderAnotherFriesChance = 0.3;
const double orderAnotherDrinkChance = 0.2;

const double payFailed = 0.05;
const double eatInMCD = 0.5;
const double denyTable = 0.1;
const double cancelMeal = 0.05;


const double packRemains = 0.8;
const double badClient = 0.5;

//
// Workers parameters
//

const NormalParams assessTime = {.center = 0.08, .scattering = 0.015};

// Lobby workers parameters
const NormalParams tableCleaningTime = {.center = 0.25, .scattering = 0.05};

// Service workers parameters
const NormalParams packAnOrderTime = {.center = 1.5, .scattering = 0.2};
const NormalParams orderExtraditionTime = {.center = 1, .scattering = 0.2};

// Frier parameters 
const int minimalFries = 5;
const int frierSlotsCount = 4;
const int friesPortionsInSlot = 4;

const NormalParams friesPackingTime = {.center = 0.1, .scattering = 0.01};
const NormalParams friesTransferTime = {.center = 0.1, .scattering = 0.01};
const NormalParams friesFryingPrepareTime = {.center = 0.2, .scattering = 0.02};
const double friesPreparingTime = 2.2;

// Beverage parameters
const int drinkDistributorCount = 5;

const NormalParams drinkPreparingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams drinkPackingTime = {.center = 0.2, .scattering = 0.02};
const double drinkFillingTime = 0.2;

// Kitchen paraneters
const int rawMinimum = 5;
const int rawFryerCount = 6;
const int grillPlots = 4;
const int meatMinimum = 12;
const int meatOnOnePlot = 6;

const NormalParams bunsTransferTime = {.center = 0.2, .scattering = 0.02};
const NormalParams bunsToToasterTime = {.center = 0.2, .scattering = 0.02};
const NormalParams singleBurgerFillingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams doubleBurgerFillingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams singleBurgerPackingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams doubleBurgerPackingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams addPackingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams addOrderingTime = {.center = 0.2, .scattering = 0.02};
const NormalParams rawToTrayTime = {.center = 0.2, .scattering = 0.02};
const NormalParams rawToFryerTime = {.center = 0.2, .scattering = 0.02};
const NormalParams meatPreparingStartTime = {.center = 0.2, .scattering = 0.02};
const NormalParams meatTransferTime = {.center = 0.2, .scattering = 0.02};
const double meatTime = 1;
const double rawTime = 2.5;
const double bunTime = 0.36;

#endif