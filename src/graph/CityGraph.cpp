#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph/Location.cpp"
#include "graph/Edge.cpp"
using namespace std;

class CityGraph {
    unordered_map<string, Location*> locations;
    unordered_map<string, vector<Edge*>> adjacency;

    double roadDistanceMultiplier;

    double toRadians(double degrees) {
        double pi = acos(-1.0);
        return degrees * pi / 180.0;
    }

    int estimateTravelTimeMinutes(double distanceKm, double averageSpeedKmph) {
        if (averageSpeedKmph <= 0) {
            averageSpeedKmph = 1;
        }

        return (int)ceil((distanceKm / averageSpeedKmph) * 60.0);
    }

public:
    CityGraph() {
        this->roadDistanceMultiplier = 1.25;
    }

    void addLocation(Location* location) {
        locations[location->getId()] = location;

        if (!adjacency.count(location->getId())) {
            adjacency[location->getId()] = vector<Edge*>();
        }
    }

    // Haversine formula to form the complete graph
    void addRoad(Location* from, Location* to, double averageSpeedKmph) {

        double straightLineDistanceKm = calculateStraightLineDistanceKm(from, to);
        double roadDistanceKm = straightLineDistanceKm * roadDistanceMultiplier;
        int timeMinutes = estimateTravelTimeMinutes(roadDistanceKm, averageSpeedKmph);

        adjacency[from->getId()].push_back(new Edge(to, roadDistanceKm, timeMinutes));
        adjacency[to->getId()].push_back(new Edge(from, roadDistanceKm, timeMinutes));
    }

    Location* getLocation(string id) {
        return locations.count(id) ? locations[id] : nullptr;
    }

    vector<Edge*> getNeighbors(Location* location) {
        return adjacency.count(location->getId()) ? adjacency[location->getId()] : vector<Edge*>();
    }

    double calculateStraightLineDistanceKm(Location* first, Location* second) {
        double earthRadiusKm = 6371.0;

        double firstLatitude = toRadians(first->getLatitude());
        double secondLatitude = toRadians(second->getLatitude());
        double latitudeDifference = toRadians(second->getLatitude() - first->getLatitude());
        double longitudeDifference = toRadians(second->getLongitude() - first->getLongitude());

        double haversineValue = sin(latitudeDifference / 2.0) * sin(latitudeDifference / 2.0)
                              + cos(firstLatitude) * cos(secondLatitude)
                              * sin(longitudeDifference / 2.0) * sin(longitudeDifference / 2.0);
        double angularDistance = 2.0 * atan2(sqrt(haversineValue), sqrt(1.0 - haversineValue));

        return earthRadiusKm * angularDistance;
    }
};
