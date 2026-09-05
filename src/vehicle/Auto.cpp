#pragma once
#include "vehicle/Vehicle.cpp"

class Auto : public Vehicle {
public:
    Auto() {
        this->typeName = "Auto";
        this->baseFare = 25;
        this->perKmRate = 8;
        this->perMinuteRate = 1.5;
        this->capacity = 3;
    }
};
