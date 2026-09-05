#pragma once
#include "vehicle/Vehicle.cpp"

class SUV : public Vehicle {
public:
    SUV() {
        this->typeName = "SUV";
        this->baseFare = 80;
        this->perKmRate = 16;
        this->perMinuteRate = 2.5;
        this->capacity = 6;
    }
};
