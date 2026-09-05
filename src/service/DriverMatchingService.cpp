#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include "service/RouteService.cpp"
#include "domain/Driver.cpp"
#include "graph/Location.cpp"
#include "matching/DriverMatch.cpp"
using namespace std;

class DriverMatchingService {
    RouteService* routeService;

    class FarthestDriverMatchComparator {
    public:
        bool operator()(DriverMatch* first, DriverMatch* second) const {
            return first->getEtaMinutes() < second->getEtaMinutes();
        }
    };

public:
    DriverMatchingService(RouteService* routeService) {
        this->routeService = routeService;
    }

    vector<DriverMatch*> findNearestDrivers(
        Location* pickup,
        vector<Driver*> drivers,
        string requestedType,
        int maxDrivers
    ) {
        vector<DriverMatch*> nearestDrivers;

        if (maxDrivers <= 0) {
            return nearestDrivers;
        }

        priority_queue<DriverMatch*, vector<DriverMatch*>, FarthestDriverMatchComparator> topKDrivers;

        for (Driver* driver : drivers) {
            if (driver->getStatus() != AVAILABLE) {
                continue;
            }
            if (driver->getVehicle()->getTypeName() != requestedType) {
                continue;
            }

            RouteResult* routeToPickup = routeService->findShortestRoute(
                driver->getCurrentLocation(),
                pickup
            );

            if (!routeToPickup->isFound()) {
                continue;
            }

            DriverMatch* candidate = new DriverMatch(driver, routeToPickup->getTotalTimeMinutes());

            if ((int)topKDrivers.size() < maxDrivers) {
                topKDrivers.push(candidate);
                continue;
            }

            if (candidate->getEtaMinutes() < topKDrivers.top()->getEtaMinutes()) {
                topKDrivers.pop();
                topKDrivers.push(candidate);
            }
        }

        while (!topKDrivers.empty()) {
            nearestDrivers.push_back(topKDrivers.top());
            topKDrivers.pop();
        }

        reverse(nearestDrivers.begin(), nearestDrivers.end());
         return nearestDrivers;
    }
};
