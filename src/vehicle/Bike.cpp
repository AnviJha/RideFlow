#pragma once
#include "vehicle/Vehicle.cpp"

class Bike : public Vehicle {
public:
    Bike() {
        this->typeName = "Bike";
        this->baseFare = 15;
        this->perKmRate = 6;
        this->perMinuteRate = 1.0;
        this->capacity = 1;
    }
};
