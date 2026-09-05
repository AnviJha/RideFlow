#pragma once
#include <vector>
#include <iostream>
#include "domain/Driver.cpp"
#include "domain/Ride.cpp"
#include "matching/DriverMatch.cpp"
using namespace std;

class NotificationService {
public:
    NotificationService() {
    }

    void notifyDriver(Driver* driver, Ride* ride, int etaMinutes) {
        cout << "[Notify] " << driver->getName()
             << " (" << driver->getVehicle()->getTypeName() << ")"
             << " -- ride for " << ride->getRider()->getName()
             << ": " << ride->getSource()->getName()
             << " -> " << ride->getDestination()->getName()
             << " (" << etaMinutes << " min away)" << endl;
    }

    void notifyDrivers(vector<DriverMatch*> matches, Ride* ride) {
        for (DriverMatch* match : matches) {
            notifyDriver(match->getDriver(), ride, match->getEtaMinutes());
        }
    }
};
