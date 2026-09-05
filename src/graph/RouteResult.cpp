#pragma once
#include <vector>
#include "graph/Location.cpp"
using namespace std;

class RouteResult {
    vector<Location*> path;
    double totalDistanceKm;
    int totalTimeMinutes;
    bool found;

public:
    RouteResult() {
        this->totalDistanceKm = 0.0;
        this->totalTimeMinutes = 0;
        this->found = false;
    }

    void setPath(vector<Location*> path) { this->path = path; }
    vector<Location*> getPath() { return path; }
    void setTotalDistanceKm(double totalDistanceKm) { this->totalDistanceKm = totalDistanceKm; }
    double getTotalDistanceKm() { return totalDistanceKm; }
    void setTotalTimeMinutes(int totalTimeMinutes) { this->totalTimeMinutes = totalTimeMinutes; }
    int getTotalTimeMinutes() { return totalTimeMinutes; }
    void setFound(bool found) { this->found = found; }
    bool isFound() { return found; }
};
