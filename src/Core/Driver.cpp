//
// Created by hp on 11/30/2025.
//

#include "Driver.h"

Driver::Driver() {
    name = "Default";
    team = "idk gng";
    points = 0;
    price = 0;
}

Driver::Driver(string Code, string Name, string Team, int Points, float Price, int Finishes, int Wins, int Podiums) {
    code = Code;
    name = Name;
    team = Team;
    points = Points;
    price = Price;
    avgfinishes = Finishes;
    wins = Wins;
    podiums = Podiums;
}

string Driver::getName() const{
    return name;
}
string Driver::getCode() const{
    return code;
}
string Driver::getTeam() const{
    return team;
}
int Driver::getPoints() const{
    return points;
}
float Driver::getPrice() const{
    return price;
}

void Driver::addpoints(int x) {
    points += x;
}




