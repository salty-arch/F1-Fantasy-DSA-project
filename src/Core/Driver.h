//
// Created by hp on 11/30/2025.
//
#include <string>
using namespace std;
#ifndef F1_FANTASY_DSA_DRIVER_H
#define F1_FANTASY_DSA_DRIVER_H


class Driver {
private:
    string code;
    string name;
    string team;
    int points;
    float price;
    int avgfinishes;
    int wins;
    int podiums;

public:
    Driver(string Code, string Name, string Team, int Points, float Price, int Finishes, int Wins, int Podiums);
    Driver();

    string getName() const;
    string getCode() const;
    string getTeam() const;
    int getPoints() const;
    float getPrice() const;

    void addpoints(int x);

};


#endif //F1_FANTASY_DSA_DRIVER_H