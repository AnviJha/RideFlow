#pragma once
#include <string>
#include "graph/Location.cpp"
#include "vehicle/Vehicle.cpp"
using namespace std;

enum DriverStatus { AVAILABLE, ON_TRIP, OFFLINE };

class Driver {
    string id;
    string name;
    Location* currentLocation;
    Vehicle* vehicle;
    DriverStatus status;

public:
    Driver(string id, string name, Location* currentLocation, Vehicle* vehicle) {
        this->id = id;
        this->name = name;
        this->currentLocation = currentLocation;
        this->vehicle = vehicle;
        this->status = AVAILABLE;
    }

    string getId() { return id; }
    string getName() { return name; }
    Location* getCurrentLocation() { return currentLocation; }
    void setCurrentLocation(Location* currentLocation) { this->currentLocation = currentLocation; }
    Vehicle* getVehicle() { return vehicle; }
    DriverStatus getStatus() { return status; }
    void setStatus(DriverStatus status) { this->status = status; }
};
