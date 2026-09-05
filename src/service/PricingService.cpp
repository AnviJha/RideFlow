#pragma once
#include "pricing/PricingStrategy.cpp"
using namespace std;

class PricingService {
    PricingStrategy* strategy;

public:
    PricingService(PricingStrategy* strategy) {
        this->strategy = strategy;
    }

    void setStrategy(PricingStrategy* strategy) {
        this->strategy = strategy;
    }

    double estimateFare(Vehicle* vehicle, double distanceKm, int timeMinutes) {
        return strategy->calculateFare(vehicle, distanceKm, timeMinutes);
    }
};
