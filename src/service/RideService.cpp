#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "service/RouteService.cpp"
#include "service/DriverMatchingService.cpp"
#include "service/NotificationService.cpp"
#include "matching/DriverMatch.cpp"
#include "service/PricingService.cpp"
#include "factory/VehicleFactory.cpp"
#include "domain/Ride.cpp"
#include "domain/Driver.cpp"
#include "domain/Rider.cpp"
#include "graph/Location.cpp"
using namespace std;

// Orchestration class --> It calls methods of other classes. ( Leader / Manager)

class RideService {
    RouteService* routeService;
    DriverMatchingService* matchingService;
    NotificationService* notificationService;
    PricingService* pricingService;
    VehicleFactory* vehicleFactory;

    int rideCounter;
    double cancellationFee;

    void recomputeRoute(Ride* ride) {

        vector<Location*> waypoints;

        waypoints.push_back(ride->getSource());

        for (Location* stop : ride->getStops()) {
            waypoints.push_back(stop);
        }

        waypoints.push_back(ride->getDestination());

        double totalDistance = 0.0;
        int totalTime = 0;

        for (int i = 0; i + 1 < (int)waypoints.size(); i++) {
            RouteResult* leg = routeService->findShortestRoute(waypoints[i], waypoints[i + 1]);
            totalDistance += leg->getTotalDistanceKm();
            totalTime += leg->getTotalTimeMinutes();
        }

        ride->setDistanceKm(totalDistance);
        ride->setTimeMinutes(totalTime);
    }

    double estimateFare(Ride* ride) {
        return pricingService->estimateFare(
            ride->getVehicle(),
            ride->getDistanceKm(),
            ride->getTimeMinutes()
        );
    }

public:
    RideService(
        RouteService* routeService,
        DriverMatchingService* matchingService,
        NotificationService* notificationService,
        PricingService* pricingService,
        VehicleFactory* vehicleFactory
    ) {
        this->routeService = routeService;
        this->matchingService = matchingService;
        this->notificationService = notificationService;
        this->pricingService = pricingService;
        this->vehicleFactory = vehicleFactory;

        this->rideCounter = 0;
        this->cancellationFee = 30.0;
    }

    Ride* createRide(Rider* rider, string vehicleType, Location* source, Location* destination) {

        string rideId = "RIDE-" + to_string(++rideCounter);

        Vehicle* vehicle = vehicleFactory->createVehicle(vehicleType);

        Ride* ride = new Ride(rideId, rider, vehicle, source, destination);

        recomputeRoute(ride);

        ride->setFare(estimateFare(ride));

        return ride;
    }

    vector<DriverMatch*> notifyNearbyDrivers(Ride* ride, vector<Driver*> drivers, int maxDrivers) {
        vector<DriverMatch*> nearestDrivers = matchingService->findNearestDrivers(
            ride->getSource(),
            drivers,
            ride->getVehicle()->getTypeName(),
            maxDrivers
        );

        notificationService->notifyDrivers(nearestDrivers, ride);
        return nearestDrivers;
    }

    void assignDriver(Ride* ride, Driver* driver) {
        ride->setDriver(driver);
        driver->setStatus(ON_TRIP);
        ride->setStatus(DRIVER_ASSIGNED);
    }

    void startRide(Ride* ride) {
        if (ride->getStatus() != DRIVER_ASSIGNED) {
            cout << "Cannot start ride: no driver assigned yet." << endl;
            return;
        }

        ride->setStatus(IN_PROGRESS);
    }

    void addStop(Ride* ride, Location* stop) {
        if (ride->getStatus() == COMPLETED || ride->getStatus() == CANCELLED) {
            cout << "Cannot add a stop to a finished ride." << endl;
            return;
        }

        double oldFare = ride->getFare();
        ride->addStop(stop);
        recomputeRoute(ride);

        double newFare = estimateFare(ride);
        ride->setFare(newFare);

        cout << fixed << setprecision(2);
        cout << "Stop added at " << stop->getName() << "." << endl;
        cout << "New trip: " << ride->getDistanceKm() << " km, "
             << ride->getTimeMinutes() << " min" << endl;
        cout << "Fare updated: Rs " << oldFare << " -> Rs " << newFare
             << " (+Rs " << (newFare - oldFare) << ")" << endl;
    }

    double cancelRide(Ride* ride) {
        if (ride->getStatus() == COMPLETED) {
            cout << "Cannot cancel: ride already completed." << endl;
            return 0.0;
        }
        if (ride->getStatus() == CANCELLED) {
            cout << "Ride already cancelled." << endl;
            return 0.0;
        }

        double fee = 0.0;
        if (ride->getStatus() == DRIVER_ASSIGNED || ride->getStatus() == IN_PROGRESS) {
            fee = cancellationFee;
        }

        if (ride->getDriver()) {
            ride->getDriver()->setStatus(AVAILABLE);
        }

        ride->setStatus(CANCELLED);
        return fee;
    }

    void completeRide(Ride* ride) {
        if (ride->getStatus() != IN_PROGRESS) {
            cout << "Cannot complete: ride is not in progress." << endl;
            return;
        }

        ride->setStatus(COMPLETED);

        if (ride->getDriver()) {
            ride->getDriver()->setStatus(AVAILABLE);
        }
    }

    void printReceipt(Ride* ride) {
        cout << "---------------------------------------------" << endl;
        cout << "RECEIPT " << ride->getId() << endl;
        cout << "Rider: " << ride->getRider()->getName() << endl;

        if (ride->getDriver()) {
            cout << "Driver: " << ride->getDriver()->getName()
                 << " (" << ride->getVehicle()->getTypeName() << ")" << endl;
        }

        cout << "From: " << ride->getSource()->getName() << endl;

        for (Location* stop : ride->getStops()) {
            cout << "Stop: " << stop->getName() << endl;
        }

        cout << "To: " << ride->getDestination()->getName() << endl;
        cout << fixed << setprecision(2);
        cout << "Distance: " << ride->getDistanceKm() << " km" << endl;
        cout << "Time: " << ride->getTimeMinutes() << " min" << endl;
        cout << "Status: " << rideStatusName(ride->getStatus()) << endl;
        cout << "Fare: Rs " << ride->getFare() << endl;
        cout << "---------------------------------------------" << endl;
    }
};
