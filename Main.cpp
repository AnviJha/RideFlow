#include <iostream>
#include <vector>
#include <iomanip>

#include "graph/CityGraph.cpp"
#include "graph/Location.cpp"
#include "service/RouteService.cpp"
#include "vehicle/Vehicle.cpp"
#include "vehicle/Bike.cpp"
#include "vehicle/Auto.cpp"
#include "vehicle/Sedan.cpp"
#include "vehicle/SUV.cpp"
#include "factory/VehicleFactory.cpp"
#include "pricing/PricingStrategy.cpp"
#include "pricing/NormalPricing.cpp"
#include "pricing/SurgePricing.cpp"
#include "pricing/PoolPricing.cpp"
#include "service/PricingService.cpp"
#include "domain/Driver.cpp"
#include "domain/Rider.cpp"
#include "domain/Ride.cpp"
#include "matching/DriverMatch.cpp"
#include "service/DriverMatchingService.cpp"
#include "service/NotificationService.cpp"
#include "service/RideService.cpp"

using namespace std;

int main() {
    CityGraph* city = new CityGraph();

    Location* cp = new Location("CP", "Connaught Place", 28.6315, 77.2167);
    Location* kb = new Location("KB", "Karol Bagh", 28.6519, 77.1907);
    Location* hk = new Location("HK", "Hauz Khas", 28.5494, 77.2001);
    Location* saket = new Location("SAKET", "Saket", 28.5245, 77.2066);
    Location* dwarka = new Location("DWARKA", "Dwarka", 28.5921, 77.0460);
    Location* noida = new Location("NOIDA", "Noida Sector 18", 28.5708, 77.3261);

    city->addLocation(cp);
    city->addLocation(kb);
    city->addLocation(hk);
    city->addLocation(saket);
    city->addLocation(dwarka);
    city->addLocation(noida);

    city->addRoad(cp, kb, 24.0);
    city->addRoad(cp, hk, 30.0);
    city->addRoad(kb, hk, 28.0);
    city->addRoad(hk, saket, 22.0);
    city->addRoad(cp, noida, 32.0);
    city->addRoad(hk, noida, 35.0);
    city->addRoad(kb, dwarka, 34.0);
    city->addRoad(hk, dwarka, 36.0);
    city->addRoad(saket, noida, 38.0);

    RouteService* routeService = new RouteService(city);

    VehicleFactory* vehicleFactory = new VehicleFactory();

    PricingStrategy* normalPricing = new NormalPricing();
    PricingStrategy* surgePricing = new SurgePricing(1.5);
    PricingStrategy* poolPricing = new PoolPricing(0.75);

    PricingService* pricingService = new PricingService(normalPricing);

    NotificationService* notificationService = new NotificationService();

    DriverMatchingService* matchingService = new DriverMatchingService(routeService);

    RideService* rideService = new RideService(
        routeService,
        matchingService,
        notificationService,
        pricingService,
        vehicleFactory
    );

    // ------

    vector<Driver*> drivers;
    drivers.push_back(new Driver("D1", "Ramesh", kb, vehicleFactory->createVehicle("SEDAN")));
    drivers.push_back(new Driver("D2", "Suresh", hk, vehicleFactory->createVehicle("SEDAN")));
    drivers.push_back(new Driver("D3", "Ganesh", noida, vehicleFactory->createVehicle("SEDAN")));
    drivers.push_back(new Driver("D4", "Mahesh", dwarka, vehicleFactory->createVehicle("SUV")));
    drivers.push_back(new Driver("D5", "Vikram", kb, vehicleFactory->createVehicle("BIKE")));

    Driver* dinesh = new Driver("D6", "Dinesh", hk, vehicleFactory->createVehicle("SEDAN"));
    dinesh->setStatus(ON_TRIP);
    drivers.push_back(dinesh);

    Rider* asha = new Rider("R1", "Asha");

    // Happy Flow :

    cout << "=== 1. Book a ride ===" << endl;

    Ride* ride = rideService->createRide(asha, "SEDAN", cp, saket);

    cout << "Requested: " << ride->getSource()->getName() << " -> "
         << ride->getDestination()->getName()
         << " in a " << ride->getVehicle()->getTypeName() << endl;
    cout << fixed << setprecision(2);
    cout << "Trip: " << ride->getDistanceKm() << " km, " << ride->getTimeMinutes() << " min" << endl;

    cout << "=== 2. Find & notify top 2 nearest Sedan drivers ===" << endl;
    vector<DriverMatch*> matches = rideService->notifyNearbyDrivers(ride, drivers, 2);
    cout << endl;

    cout << "=== 3. Assign & start ===" << endl;
    Driver* acceptedDriver = matches[0]->getDriver();

    rideService->assignDriver(ride, acceptedDriver);
    cout << acceptedDriver->getName() << " accepted. Status: " << rideStatusName(ride->getStatus()) << endl;

    rideService->startRide(ride);
    cout << "Status: " << rideStatusName(ride->getStatus()) << endl;
    cout << endl;

    // Total fare before add stop
    cout << "Fare before adding stop: Rs " << ride->getFare() << endl;

    cout << "=== 4. Add a stop mid-ride ===" << endl;
    rideService->addStop(ride, kb);
    cout << endl;

    // Total fare after add stop
    cout << "Fare after adding stop : Rs " << ride->getFare() << endl;

    cout << "=== 5. Complete the ride ===" << endl;
    rideService->completeRide(ride);

    rideService->printReceipt(ride);

    cout << endl;

    // Happy flow finished --------->

    cout << "=== 6. A second ride that gets cancelled ===" << endl;
    Rider* bhuvan = new Rider("R2", "Bhuvan");

    Ride* ride2 = rideService->createRide(bhuvan, "SEDAN", cp, noida);
    vector<DriverMatch*> matches2 = rideService->notifyNearbyDrivers(ride2, drivers, 2);

    if (!matches2.empty()) {
        rideService->assignDriver(ride2, matches2[0]->getDriver());
        cout << matches2[0]->getDriver()->getName() << " assigned." << endl;
    }

    double fee = rideService->cancelRide(ride2);
    cout << fixed << setprecision(2);
    cout << "Rider cancelled after assignment. Status: " << rideStatusName(ride2->getStatus())
         << ". Cancellation fee: Rs " << fee << endl;

    return 0;
}
