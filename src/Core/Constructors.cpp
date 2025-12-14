//
// Created by hp on 11/30/2025.
//

#include "Constructors.h"

Constructors::Constructors(string Code, string Team, float Price) {
    code = Code;
    Team_name = Team;
    price = Price;
}

Constructors::Constructors() {
    code = "Default";
    Team_name = "DefaultTeam";
    points = 0;
    price = 0.0f;
}

string Constructors::getCode() const {  // Add const here
    return code;
}

int Constructors::getPoints() const {  // Add const here
    return points;
}

float Constructors::getPrice() const {  // Add const here
    return price;
}

string Constructors::getTeamName() const {  // Add const here
    return Team_name;
}

// addpoints() stays the same (no const because it modifies)
void Constructors::addpoints(int p) {
    points += p;
}





