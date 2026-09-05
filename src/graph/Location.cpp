#pragma once
#include <string>
using namespace std;

class Location {
    string id;
    string name;
    double latitude;
    double longitude;

public:
    Location(string id, string name, double latitude, double longitude) {
        this->id = id;
        this->name = name;
        this->latitude = latitude;
        this->longitude = longitude;
    }

    string getId() { return id; }
    string getName() { return name; }
    double getLatitude() { return latitude; }
    double getLongitude() { return longitude; }
};
