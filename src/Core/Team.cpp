//
// Created by hp on 11/30/2025.
//
#include <string>
using namespace std;
#include "Team.h"


Team::Team(float Budgetlimit, Constructors Constructor){
    this->budgetlimit = Budgetlimit;
    this->constructor = Constructor;
}

Team::Team() {
    this->budgetlimit = 0.0;
    this->constructor = Constructors();
}


void Team::addDriver(Driver driver) {
    Drivers.push_back(driver);
    totaldrivers++;
}

void Team::removeDriver(Driver driver) {
    for (int i = 0; i < Drivers.size(); i++) {
        if (Drivers[i].getName() == driver.getName()) {
            // swap with last and pop
            Drivers[i] = Drivers.back();
            Drivers.pop_back();
            return;
        }
    }
}

void Team::setConstructor(Constructors Constructor) {
    constructor = Constructor;
}

float Team::getBudgetLimit() {
    return budgetlimit;
}

Constructors Team::getConstructor() {
    return constructor;
}


vector<Driver> Team::getDrivers() {
    return Drivers;
}







