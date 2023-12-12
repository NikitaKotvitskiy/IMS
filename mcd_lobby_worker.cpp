#include <iostream>
#include "simlib.h"
#include "mcd.h"

void LobbyWorker::Behavior() {
    while (true)
    {
        Wait(Normal(assessTime.center, assessTime.scattering));

        for (int i = 0; i < tableCount; i++)
            if (tables[i].dirty && !(tables[i].inService)) {
                cout << Time << ": lobby worker started cleaning table " << i << endl;
                tables[i].inService = true;
                Wait(Normal(tableCleaningTime.center, tableCleaningTime.scattering));
                tables[i].inService = false;
                tables[i].dirty = false;
                cout << Time << ": lobby worker finished cleaning table " << i << endl;
            }
    }
}