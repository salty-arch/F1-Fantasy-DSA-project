//
// Leaderboard System for Player Rankings
// Uses unordered_map for fast lookup and priority_queue for rankings
//

#ifndef F1_FANTASY_DSA_LEADERBOARD_H
#define F1_FANTASY_DSA_LEADERBOARD_H

#include "../Core/User.h"
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct LeaderboardEntry {
    string username;
    int points;
    float budget;
    int rank;

    LeaderboardEntry(string name, int pts, float bdgt, int r = 0)
        : username(name), points(pts), budget(bdgt), rank(r) {}
};

// Comparator for max-heap (higher points = higher priority)
struct ComparePoints {
    bool operator()(const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.points != b.points) {
            return a.points < b.points;  // Max heap by points
        }
        return a.username > b.username;  // Tiebreaker: alphabetical
    }
};

class Leaderboard {
private:
    unordered_map<string, User*> users;  // Fast O(1) lookup by username
    vector<LeaderboardEntry> sortedRankings;  // Stable sorted list

    // Helper functions
    void rebuildRankings();
    void sortRankings();

public:
    Leaderboard();

    // User management
    void addUser(User* user);
    void removeUser(const string& username);
    User* getUser(const string& username);
    bool userExists(const string& username);

    // Points updates
    void updateUserPoints(const string& username, int points);
    void addPointsToUser(const string& username, int points);

    // Rankings
    vector<LeaderboardEntry> getTopN(int n);
    vector<LeaderboardEntry> getAllRankings();
    int getUserRank(const string& username);

    // Statistics
    int getTotalUsers() const;
    int getAveragePoints() const;
    LeaderboardEntry getLeader() const;

    // Display
    void displayTopN(int n = 10);
    void displayFullLeaderboard();
    void displayUserStats(const string& username);

    // File operations
    bool exportToFile(const string& filename);
    bool loadFromFile(const string& filename);

    // Utility
    void clear();
};

#endif //F1_FANTASY_DSA_LEADERBOARD_H