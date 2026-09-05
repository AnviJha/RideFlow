#pragma once
#include "domain/Driver.cpp"

class DriverMatch {
    Driver* driver;
    int etaMinutes;

public:
    DriverMatch(Driver* driver, int etaMinutes) {
        this->driver = driver;
        this->etaMinutes = etaMinutes;
    }

    Driver* getDriver() { return driver; }
    int getEtaMinutes() { return etaMinutes; }
};
