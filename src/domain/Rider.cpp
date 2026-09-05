#pragma once
#include <string>
using namespace std;

class Rider {
    string id;
    string name;

public:
    Rider(string id, string name) {
        this->id = id;
        this->name = name;
    }

    string getId() { return id; }
    string getName() { return name; }
};
