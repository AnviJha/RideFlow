#pragma once
#include "graph/Location.cpp"

class Edge {
    Location* destination;
    double distanceKm;
    int timeMinutes;

public:
    Edge(Location* destination, double distanceKm, int timeMinutes) {
        this->destination = destination;
        this->distanceKm = distanceKm;
        this->timeMinutes = timeMinutes;
    }

    Location* getDestination() { return destination; }
    double getDistanceKm() { return distanceKm; }
    int getTimeMinutes() { return timeMinutes; }
};
