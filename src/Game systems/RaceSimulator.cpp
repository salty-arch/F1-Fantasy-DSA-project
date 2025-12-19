//
// Race Simulation Engine Implementation
//

#include "RaceSimulator.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// ============ CONSTRUCTOR ============

RaceSimulator::RaceSimulator(const string& name) : raceName(name) {
    srand(time(0));
}

// ============ SETUP ============

void RaceSimulator::addDriver(Driver* driver) {
    if (driver != nullptr) {
        drivers.push_back(driver);
    }
}

// ============ RACE SIMULATION ============

void RaceSimulator::runRace() {
    if (drivers.empty()) {
        cout << "No drivers in race!" << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "    RACE: " << raceName << endl;
    cout << "    Drivers: " << drivers.size() << endl;
    cout << "========================================" << endl;
    cout << "Starting race simulation..." << endl;

    //  Simulate race positions
    simulateRacePositions();

    //  Apply random events (penalties, DNF, etc.)
    applyRandomEvents();

    //  Assign fastest lap
    assignFastestLap();

    //  Calculate points
    calculatePoints();

    cout << "Race completed!" << endl;
}

void RaceSimulator::simulateRacePositions() {
    raceResults.clear();

    // Create results with randomized positions
    // Better drivers (higher current points) have better chance of good positions
    for (Driver* driver : drivers) {
        int basePosition = rand() % drivers.size() + 1;

        // Bias towards better positions for higher-rated drivers
        if (driver->getPoints() > 150) {
            basePosition = getRandomPosition(drivers.size() / 3);
        } else if (driver->getPoints() > 100) {
            basePosition = getRandomPosition(drivers.size() / 2);
        }

        RaceResult result(driver, basePosition);
        raceResults.push_back(result);
    }

    // Sort by position
    sort(raceResults.begin(), raceResults.end(),
         [](const RaceResult& a, const RaceResult& b) {
             return a.position < b.position;
         });

    // Ensure unique positions
    for (int i = 0; i < raceResults.size(); i++) {
        raceResults[i].position = i + 1;
    }
}

void RaceSimulator::applyRandomEvents() {
    // 20% chance of penalty for random driver
    if (rand() % 100 < 20) {
        int penalizedIdx = rand() % raceResults.size();
        cout << "  PENALTY: " << raceResults[penalizedIdx].driver->getName()
             << " receives 5-second penalty!" << endl;

        // Move driver down positions
        if (penalizedIdx < raceResults.size() - 2) {
            swap(raceResults[penalizedIdx], raceResults[penalizedIdx + 2]);
            raceResults[penalizedIdx].position = penalizedIdx + 1;
            raceResults[penalizedIdx + 2].position = penalizedIdx + 3;
        }
    }

    // 10% chance of DNF for a driver in bottom half
    if (rand() % 100 < 10 && raceResults.size() > 5) {
        int dnfIdx = raceResults.size() / 2 + rand() % (raceResults.size() / 2);
        cout << " DNF: " << raceResults[dnfIdx].driver->getName()
             << " did not finish!" << endl;

        // Move to last position
        RaceResult dnfResult = raceResults[dnfIdx];
        raceResults.erase(raceResults.begin() + dnfIdx);
        raceResults.push_back(dnfResult);

        // Update positions
        for (int i = 0; i < raceResults.size(); i++) {
            raceResults[i].position = i + 1;
        }
    }
}

void RaceSimulator::assignFastestLap() {
    if (raceResults.empty()) return;

    // Use priority queue to find drivers in points positions
    priority_queue<RaceResult, vector<RaceResult>, ComparePosition> topDrivers;

    for (const RaceResult& result : raceResults) {
        if (result.position <= 10) {  // Only top 10 can get fastest lap points
            topDrivers.push(result);
        }
    }

    // Higher chance for top 5 drivers to get fastest lap
    int fastestLapPos = rand() % 5;

    if (!topDrivers.empty() && fastestLapPos < raceResults.size()) {
        raceResults[fastestLapPos].fastestLap = true;
        cout << " FASTEST LAP: " << raceResults[fastestLapPos].driver->getName() << endl;
    }
}

void RaceSimulator::calculatePoints() {
    for (RaceResult& result : raceResults) {
        int points = 0;

        // Position points (top 10 only)
        if (result.position <= 10) {
            points = POINTS_TABLE[result.position - 1];
        }

        // Fastest lap bonus (must finish in top 10)
        if (result.fastestLap && result.position <= 10) {
            points += FASTEST_LAP_POINTS;
        }

        result.pointsEarned = points;

        // Update driver's total points
        if (result.driver != nullptr) {
            result.driver->addpoints(points);
        }
    }
}

int RaceSimulator::getRandomPosition(int maxPos) {
    return rand() % maxPos + 1;
}

// ============ DISPLAY FUNCTIONS ============

void RaceSimulator::displayResults() {
    if (raceResults.empty()) {
        cout << "No race results available!" << endl;
        return;
    }

    cout << "\n=== RACE RESULTS: " << raceName << " ===" << endl;
    cout << "Pos | Driver                | Team           | Points" << endl;
    cout << "-----------------------------------------------------" << endl;

    for (const RaceResult& result : raceResults) {
        cout << setw(3) << result.position << " | "
             << setw(21) << left << result.driver->getName() << " | "
             << setw(14) << result.driver->getTeam() << " | "
             << setw(2) << right << result.pointsEarned;

        if (result.fastestLap) {
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void RaceSimulator::displayDetailedResults() {
    if (raceResults.empty()) {
        cout << "No race results available!" << endl;
        return;
    }

    cout << "\n=== DETAILED RACE RESULTS ===" << endl;
    cout << "Race: " << raceName << endl;
    cout << "Total Drivers: " << raceResults.size() << endl;

    // Winner
    cout << "\n WINNER: " << raceResults[0].driver->getName()
         << " (" << raceResults[0].driver->getTeam() << ")" << endl;

    // Podium
    cout << "\n Podium:" << endl;
    for (int i = 0; i < 3 && i < raceResults.size(); i++) {
        cout << " P" << (i+1) << ": "
             << raceResults[i].driver->getName()
             << " - " << raceResults[i].pointsEarned << " pts" << endl;
    }

    // Fastest lap
    for (const RaceResult& result : raceResults) {
        if (result.fastestLap) {
            cout << "\n Fastest Lap: " << result.driver->getName() << endl;
            break;
        }
    }

    // Points scorers
    cout << "\n Points Scorers:" << endl;
    int pointsCount = 0;
    for (const RaceResult& result : raceResults) {
        if (result.pointsEarned > 0) {
            pointsCount++;
        }
    }
    cout << pointsCount << " drivers scored points" << endl;
}

// ============ GETTERS ============

vector<RaceResult> RaceSimulator::getResults() const {
    return raceResults;
}

RaceResult RaceSimulator::getWinner() const {
    if (!raceResults.empty()) {
        return raceResults[0];
    }
    return RaceResult(nullptr, 0);
}

Driver* RaceSimulator::getFastestLapDriver() const {
    for (const RaceResult& result : raceResults) {
        if (result.fastestLap) {
            return result.driver;
        }
    }
    return nullptr;
}

// ============ UTILITY ============

void RaceSimulator::reset() {
    raceResults.clear();
    drivers.clear();
}