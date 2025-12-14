//
// Created by hp on 11/30/2025.
//
#include <string>

#include "../Core/Team.h"
using namespace std;
#ifndef F1_FANTASY_DSA_USER_H
#define F1_FANTASY_DSA_USER_H


class User {
private:
    string username;
    int points = 0;
    float budget;
    Team team = Team();

    public:
    User();
    User(string username, float Budget, Team team);

    string getUsername() const;
    int getPoints() const;
    float getBudget() const;
    Team getTeam() const;

    void addPoints(int p);
    void setPoints(int p);
};


#endif //F1_FANTASY_DSA_USER_H