#pragma once
#include <iostream>
#include <string>
#include "vehicle/Bike.cpp"
#include "vehicle/Auto.cpp"
#include "vehicle/Sedan.cpp"
#include "vehicle/SUV.cpp"
using namespace std;

class VehicleFactory {
public:
    VehicleFactory() {
    }

    Vehicle* createVehicle(string type) {
        if (type == "BIKE") {
            return new Bike();
        }
        if (type == "AUTO") {
            return new Auto();
        }
        if (type == "SEDAN") {
            return new Sedan();
        }
        if (type == "SUV") {
            return new SUV();
        }
        cout << "Unknown vehicle type '" << type << "', defaulting to Sedan." << endl;
        return new Sedan();
    }
};
