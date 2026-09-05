#pragma once
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "graph/CityGraph.cpp"
#include "graph/RouteResult.cpp"
using namespace std;

class RouteService {
    CityGraph* graph;

    double optimisticSpeedKmph;

    class RouteQueueEntry {
        string locationId;
        int timeFromSource;
        double estimatedTotalTime;

    public:
        RouteQueueEntry(string locationId, int timeFromSource, double estimatedTotalTime) {
            this->locationId = locationId;
            this->timeFromSource = timeFromSource;
            this->estimatedTotalTime = estimatedTotalTime;
        }

        string getLocationId() const {
            return locationId;
        }

        int getTimeFromSource() const {
            return timeFromSource;
        }

        double getEstimatedTotalTime() const {
            return estimatedTotalTime;
        }
    };

    class RouteQueueEntryComparator {
    public:
        bool operator()(const RouteQueueEntry& first, const RouteQueueEntry& second) const {
            return first.getEstimatedTotalTime() > second.getEstimatedTotalTime();
        }
    };

    double estimateRemainingTime(Location* from, Location* destination) {
        double straightLineDistanceKm = graph->calculateStraightLineDistanceKm(from, destination);
        return (straightLineDistanceKm / optimisticSpeedKmph) * 60.0;
    }

public:
    RouteService(CityGraph* graph) {
        this->graph = graph;
        this->optimisticSpeedKmph = 80.0;
    }

    // uses A* algorithm to find the shortest route between two locations
    RouteResult* findShortestRoute(Location* source, Location* destination) {
        unordered_map<string, int> bestTime;
        unordered_map<string, double> bestDistance;
        unordered_map<string, Location*> previous;

        priority_queue<RouteQueueEntry, vector<RouteQueueEntry>, RouteQueueEntryComparator> openSet;

        double sourceEstimate = estimateRemainingTime(source, destination);

        bestTime[source->getId()] = 0;
        bestDistance[source->getId()] = 0.0;
        openSet.push(RouteQueueEntry(source->getId(), 0, sourceEstimate));

        while (!openSet.empty()) {
            RouteQueueEntry current = openSet.top();
            openSet.pop();

            if (current.getTimeFromSource() > bestTime[current.getLocationId()]) {
                continue;
            }
            if (current.getLocationId() == destination->getId()) {
                break;
            }

            Location* currentLocation = graph->getLocation(current.getLocationId());

            for (Edge* edge : graph->getNeighbors(currentLocation)) {
                Location* neighbor = edge->getDestination();
                string neighborId = neighbor->getId();
                int newTime = current.getTimeFromSource() + edge->getTimeMinutes();

                if (!bestTime.count(neighborId) || newTime < bestTime[neighborId]) {
                    bestTime[neighborId] = newTime;
                    bestDistance[neighborId] = bestDistance[current.getLocationId()] + edge->getDistanceKm();
                    previous[neighborId] = currentLocation;

                    double estimatedTotalTime = newTime + estimateRemainingTime(neighbor, destination);
                    openSet.push(RouteQueueEntry(neighborId, newTime, estimatedTotalTime));
                }
            }
        }

        RouteResult* result = new RouteResult();
        if (!bestTime.count(destination->getId())) {
            result->setFound(false);
            return result;
        }

        result->setFound(true);
        result->setTotalTimeMinutes(bestTime[destination->getId()]);
        result->setTotalDistanceKm(bestDistance[destination->getId()]);

        vector<Location*> reversedPath;
        for (Location* step = destination; step != nullptr; ) {
            reversedPath.push_back(step);

            if (step->getId() == source->getId()) {
                break;
            }

            step = previous.count(step->getId()) ? previous[step->getId()] : nullptr;
        }

        result->setPath(vector<Location*>(reversedPath.rbegin(), reversedPath.rend()));
        return result;
    }
};
