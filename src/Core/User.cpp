//
// Created by hp on 11/30/2025.
//

#include "User.h"
#include "Team.h"

User::User(string username, float Budget, Team team) {
    this->username = username;
    this->budget = Budget;
    this->team = team;
}

void User::addPoints(int p) {
    points += p;
}

string User::getUsername() const {
    return username;
}

int User::getPoints() const {
    return points;
}

float User::getBudget() const {
    return budget;
}

Team User::getTeam() const {
    return team;
}


void User::setPoints(int p) {
    points = p;
}







