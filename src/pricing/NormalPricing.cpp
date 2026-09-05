#pragma once
#include "pricing/PricingStrategy.cpp"

class NormalPricing : public PricingStrategy {
public:
    NormalPricing() {
    }

    double calculateFare(Vehicle* vehicle, double distanceKm, int timeMinutes) {
        return computeBaseFare(vehicle, distanceKm, timeMinutes);
    }
};
