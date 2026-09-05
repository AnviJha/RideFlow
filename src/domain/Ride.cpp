#pragma once
#include <string>
#include <vector>
#include "domain/Driver.cpp"
#include "domain/Rider.cpp"
#include "graph/Location.cpp"
#include "vehicle/Vehicle.cpp"
using namespace std;

enum RideStatus { REQUESTED, DRIVER_ASSIGNED, IN_PROGRESS, COMPLETED, CANCELLED };

string rideStatusName(RideStatus status) {
    if (status == REQUESTED) {
        return "REQUESTED";
    }
    if (status == DRIVER_ASSIGNED) {
        return "DRIVER_ASSIGNED";
    }
    if (status == IN_PROGRESS) {
        return "IN_PROGRESS";
    }
    if (status == COMPLETED) {
        return "COMPLETED";
    }

    return "CANCELLED";
}

class Ride {
    string id;
    Rider* rider;
    Driver* driver;
    Vehicle* vehicle;
    Location* source;
    Location* destination;
    vector<Location*> stops;
    RideStatus status;
    double distanceKm;
    int timeMinutes;
    double fare;

public:
    Ride(string id, Rider* rider, Vehicle* vehicle, Location* source, Location* destination) {
        this->id = id;
        this->rider = rider;
        this->vehicle = vehicle;
        this->source = source;
        this->destination = destination;
        this->driver = nullptr;
        this->status = REQUESTED;
        this->distanceKm = 0.0;
        this->timeMinutes = 0;
        this->fare = 0.0;
    }

    string getId() { return id; }
    Rider* getRider() { return rider; }
    Driver* getDriver() { return driver; }
    void setDriver(Driver* driver) { this->driver = driver; }
    Vehicle* getVehicle() { return vehicle; }
    Location* getSource() { return source; }
    Location* getDestination() { return destination; }
    void addStop(Location* stop) { stops.push_back(stop); }
    vector<Location*> getStops() { return stops; }
    RideStatus getStatus() { return status; }
    void setStatus(RideStatus status) { this->status = status; }
    double getDistanceKm() { return distanceKm; }
    void setDistanceKm(double distanceKm) { this->distanceKm = distanceKm; }
    int getTimeMinutes() { return timeMinutes; }
    void setTimeMinutes(int timeMinutes) { this->timeMinutes = timeMinutes; }
    double getFare() { return fare; }
    void setFare(double fare) { this->fare = fare; }
};
