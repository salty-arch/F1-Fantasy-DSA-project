//
// Leaderboard Implementation
//

#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "../Core/Team.h"
#include "../Core/Driver.h"
// ============ CONSTRUCTOR ============

Leaderboard::Leaderboard() {}

// ============ USER MANAGEMENT ============

void Leaderboard::addUser(User* user) {
    if (user == nullptr) return;

    string username = user->getUsername();

    // Check if user already exists
    if (users.find(username) != users.end()) {
        cout << "User '" << username << "' already exists in leaderboard!" << endl;
        return;
    }

    users[username] = user;
    rebuildRankings();
}

void Leaderboard::removeUser(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        users.erase(it);
        rebuildRankings();
    }
}

User* Leaderboard::getUser(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}

bool Leaderboard::userExists(const string& username) {
    return users.find(username) != users.end();
}

// ============ POINTS UPDATES ============

void Leaderboard::updateUserPoints(const string& username, int points) {
    User* user = getUser(username);
    if (user != nullptr) {
        // This would require adding a setPoints method to User class
        // For now, we'll just rebuild rankings
        rebuildRankings();
    }
}

void Leaderboard::addPointsToUser(const string& username, int points) {
    User* user = getUser(username);
    if (user != nullptr) {
        user->addPoints(points);
        rebuildRankings();
    }
}

// ============ RANKINGS ============

void Leaderboard::rebuildRankings() {
    sortedRankings.clear();

    // Build entries from users map
    for (auto& pair : users) {
        User* user = pair.second;
        sortedRankings.push_back(
            LeaderboardEntry(user->getUsername(), user->getPoints(), user->getBudget())
        );
    }

    sortRankings();
}

void Leaderboard::sortRankings() {
    // Sort by points (descending), then by username (ascending)
    sort(sortedRankings.begin(), sortedRankings.end(),
         [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
             if (a.points != b.points) {
                 return a.points > b.points;
             }
             return a.username < b.username;
         });

    // Assign ranks
    for (int i = 0; i < sortedRankings.size(); i++) {
        sortedRankings[i].rank = i + 1;
    }
}

vector<LeaderboardEntry> Leaderboard::getTopN(int n) {
    rebuildRankings();  // Ensure up-to-date

    vector<LeaderboardEntry> topN;
    int limit = min(n, (int)sortedRankings.size());

    for (int i = 0; i < limit; i++) {
        topN.push_back(sortedRankings[i]);
    }

    return topN;
}

vector<LeaderboardEntry> Leaderboard::getAllRankings() {
    rebuildRankings();
    return sortedRankings;
}

int Leaderboard::getUserRank(const string& username) {
    rebuildRankings();

    for (const LeaderboardEntry& entry : sortedRankings) {
        if (entry.username == username) {
            return entry.rank;
        }
    }

    return -1;  // User not found
}

// ============ STATISTICS ============

int Leaderboard::getTotalUsers() const {
    return users.size();
}

int Leaderboard::getAveragePoints() const {
    if (users.empty()) return 0;

    int totalPoints = 0;
    for (const auto& pair : users) {
        totalPoints += pair.second->getPoints();
    }

    return totalPoints / users.size();
}

LeaderboardEntry Leaderboard::getLeader() const {
    if (sortedRankings.empty()) {
        return LeaderboardEntry("", 0, 0);
    }
    return sortedRankings[0];
}

// ============ DISPLAY ============

void Leaderboard::displayTopN(int n) {
    vector<LeaderboardEntry> topN = getTopN(n);

    if (topN.empty()) {
        cout << "No users in leaderboard!" << endl;
        return;
    }

    cout << "\n=== TOP " << min(n, (int)topN.size()) << " PLAYERS ===" << endl;
    cout << "Rank | Username              | Points | Budget" << endl;
    cout << "---------------------------------------------------" << endl;

    for (const LeaderboardEntry& entry : topN) {

        cout << setw(4) << entry.rank << " | "
             << setw(21) << left << entry.username << " | "
             << setw(6) << right << entry.points << " | "
             << setw(6) << entry.budget << "M" << endl;
    }
    cout << endl;
}

void Leaderboard::displayFullLeaderboard() {
    vector<LeaderboardEntry> rankings = getAllRankings();

    if (rankings.empty()) {
        cout << "No users in leaderboard!" << endl;
        return;
    }

    cout << "\n=== FULL LEADERBOARD ===" << endl;
    cout << "Total Players: " << getTotalUsers() << endl;
    cout << "Average Points: " << getAveragePoints() << endl;
    cout << "\nRank | Username              | Points | Budget" << endl;
    cout << "---------------------------------------------------" << endl;

    for (const LeaderboardEntry& entry : rankings) {

        cout << setw(4) << entry.rank << " | "
             << setw(21) << left << entry.username << " | "
             << setw(6) << right << entry.points << " | "
             << setw(6) << entry.budget << "M" << endl;
    }
    cout << endl;
}

void Leaderboard::displayUserStats(const string& username) {
    User* user = getUser(username);
    if (user == nullptr) {
        cout << "User '" << username << "' not found!" << endl;
        return;
    }

    int rank = getUserRank(username);
    int totalUsers = getTotalUsers();
    int avgPoints = getAveragePoints();

    cout << "\n=== USER STATISTICS ===" << endl;
    cout << "Username: " << username << endl;
    cout << "Rank: " << rank << " / " << totalUsers << endl;
    cout << "Points: " << user->getPoints() << endl;
    cout << "Budget: " << user->getBudget() << "M" << endl;
    cout << "Above Average: " << (user->getPoints() > avgPoints ? "Yes" : "No") << endl;

    Team userTeam = user->getTeam();
    cout << "\nTeam Details:" << endl;
    cout << "Constructor: " << userTeam.getConstructor().getTeamName() << endl;
    cout << "Drivers: ";
    vector<Driver> drivers = userTeam.getDrivers();
    for (const Driver& d : drivers) {
        cout << d.getName() << " ";
    }
    cout << endl;
}

// ============ FILE OPERATIONS ============

bool Leaderboard::exportToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    rebuildRankings();

    // Header
    file << "=== F1 FANTASY LEAGUE LEADERBOARD ===" << endl;
    file << "Total Players: " << getTotalUsers() << endl;
    file << "Average Points: " << getAveragePoints() << endl;
    file << "Generated: [Current Date/Time]" << endl;
    file << "\nRank,Username,Points,Budget" << endl;

    // Data
    for (const LeaderboardEntry& entry : sortedRankings) {
        file << entry.rank << ","
             << entry.username << ","
             << entry.points << ","
             << entry.budget << endl;
    }

    file.close();
    cout << "Leaderboard exported to " << filename << endl;
    return true;
}

bool Leaderboard::loadFromFile(const string& filename) {
    // Implementation for loading leaderboard from file
    // This would require storing complete user data
    cout << "Load from file not yet implemented" << endl;
    return false;
}

// ============ UTILITY ============

void Leaderboard::clear() {
    users.clear();
    sortedRankings.clear();
}