#pragma once
#include "vehicle/Vehicle.cpp"

class PricingStrategy {
protected:
    double computeBaseFare(Vehicle* vehicle, double distanceKm, int timeMinutes) {
        return vehicle->getBaseFare()
             + vehicle->getPerKmRate() * distanceKm
             + vehicle->getPerMinuteRate() * timeMinutes;
    }

public:
    virtual double calculateFare(Vehicle* vehicle, double distanceKm, int timeMinutes) = 0;

    virtual ~PricingStrategy() {
    }
};
