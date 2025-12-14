//
// Created by hp on 11/30/2025.
//
#include <string>
#include <array>
#include <vector>

#include "../Core/Constructors.h"
#include "Driver.h"
using namespace std;
#ifndef F1_FANTASY_DSA_TEAM_H
#define F1_FANTASY_DSA_TEAM_H


class Team {
    vector<Driver> Drivers;
    Constructors constructor = Constructors();
    float budgetlimit;
    int totaldrivers = 0;


    public:
    Team(float budgetlimit, Constructors Constructor);
    Team();

    void addDriver(Driver driver);
    void removeDriver(Driver driver);
    void setConstructor(Constructors constructor);

    float getBudgetLimit();
    vector<Driver> getDrivers();
    Constructors getConstructor();
};


#endif //F1_FANTASY_DSA_TEAM_H