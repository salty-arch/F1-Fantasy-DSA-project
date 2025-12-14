//
// Created by hp on 11/30/2025.
//
#include <string>
using namespace std;
#ifndef F1_FANTASY_DSA_CONSTRUCTORS_H
#define F1_FANTASY_DSA_CONSTRUCTORS_H


class Constructors {
private:
    string code;
    string Team_name;
    int points = 0;
    float price;

public:
    Constructors(string Code, string Team,float Price);

    Constructors();

    string getCode() const;           // Add const
    int getPoints() const;            // Add const
    float getPrice() const;           // Add const
    string getTeamName() const;       // Add const

    void addpoints(int p);
};


#endif //F1_FANTASY_DSA_CONSTRUCTORS_H