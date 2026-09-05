#pragma once
#include "pricing/PricingStrategy.cpp"

class PoolPricing : public PricingStrategy {
    double discountFactor;

public:
    PoolPricing(double discountFactor) {
        this->discountFactor = discountFactor;
    }

    double calculateFare(Vehicle* vehicle, double distanceKm, int timeMinutes) {
        return computeBaseFare(vehicle, distanceKm, timeMinutes) * discountFactor;
    }
};
