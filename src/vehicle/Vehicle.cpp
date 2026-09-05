#pragma once
#include <string>
using namespace std;

class Vehicle {
protected:
    string typeName;
    double baseFare;
    double perKmRate;
    double perMinuteRate;
    int capacity;

public:
    Vehicle() {
        this->typeName = "";
        this->baseFare = 0;
        this->perKmRate = 0;
        this->perMinuteRate = 0;
        this->capacity = 0;
    }

    virtual ~Vehicle() {
    }

    string getTypeName() { return typeName; }
    double getBaseFare() { return baseFare; }
    double getPerKmRate() { return perKmRate; }
    double getPerMinuteRate() { return perMinuteRate; }
    int getCapacity() { return capacity; }
};
