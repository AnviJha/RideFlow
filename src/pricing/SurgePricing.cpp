#pragma once
#include "pricing/PricingStrategy.cpp"

class SurgePricing : public PricingStrategy {
    double multiplier;

public:
    SurgePricing(double multiplier) {
        this->multiplier = multiplier;
    }

    double calculateFare(Vehicle* vehicle, double distanceKm, int timeMinutes) {
        return computeBaseFare(vehicle, distanceKm, timeMinutes) * multiplier;
    }
};
