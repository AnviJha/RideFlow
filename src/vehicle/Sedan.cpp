#pragma once
#include "vehicle/Vehicle.cpp"

class Sedan : public Vehicle {
public:
    Sedan() {
        this->typeName = "Sedan";
        this->baseFare = 50;
        this->perKmRate = 12;
        this->perMinuteRate = 2.0;
        this->capacity = 4;
    }
};
