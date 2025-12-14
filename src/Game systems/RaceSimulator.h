//
// Race Simulation Engine
// Simulates F1 races with realistic scoring
//

#ifndef F1_FANTASY_DSA_RACESIMULATOR_H
#define F1_FANTASY_DSA_RACESIMULATOR_H

#include "../Core/Driver.h"
#include "../Core/Constructors.h"
#include <vector>
#include <queue>
#include <string>
using namespace std;

// Structure to hold race result for a driver
struct RaceResult {
    Driver* driver;
    int position;
    bool fastestLap;
    int pointsEarned;

    RaceResult(Driver* d, int pos)
        : driver(d), position(pos), fastestLap(false), pointsEarned(0) {}
};

// Comparator for priority queue (min-heap by position)
struct ComparePosition {
    bool operator()(const RaceResult& a, const RaceResult& b) {
        return a.position > b.position;  // Min heap
    }
};

class RaceSimulator {
private:
    vector<Driver*> drivers;
    vector<RaceResult> raceResults;
    string raceName;

    // F1 points system
    const int POINTS_TABLE[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    const int FASTEST_LAP_POINTS = 1;

    // Helper functions
    void simulateRacePositions();
    void assignFastestLap();
    void calculatePoints();
    void applyRandomEvents();
    int getRandomPosition(int basePosition);

public:
    RaceSimulator(const string& name);

    // Main operations
    void addDriver(Driver* driver);
    void runRace();
    void displayResults();
    void displayDetailedResults();

    // Getters
    vector<RaceResult> getResults() const;
    RaceResult getWinner() const;
    Driver* getFastestLapDriver() const;

    // Utility
    void reset();
};

#endif //F1_FANTASY_DSA_RACESIMULATOR_H