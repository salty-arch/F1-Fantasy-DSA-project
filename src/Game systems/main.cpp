#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>


#include "../Core/Constructors.h"
#include "../Core/Driver.h"
#include "../Core/Team.h"
#include "../Core/User.h"
#include "../dsa/Sorting.h"
#include "../dsa/AVLTree.h"
#include "../dsa/Trie.h"
#include "RaceSimulator.h"
#include "Leaderboard.h"


using namespace std;

//Loading data functions
vector<Driver> LoadDrivers(string filename);
vector<Constructors> LoadConstructors(string filename);
vector<User*> LoadUsersFromCSV(string filename, const vector<Driver>& drivers, const vector<Constructors>& constructors);
// Selecting drivers and constructor functions
Driver SelectDriver(const vector<Driver> &drivers);
Constructors SelectConstructor(const vector<Constructors> &constructors);

//Storing user team when created fucntion
bool StoreUserTeam(string filename, User user);

//Updating file functions
bool UpdateDriversCSV(string filename, const vector<Driver>& drivers);
bool UpdateUsersCSV(string filename, const vector<User*>& users);
void updateUserPointsFromRace(vector<User*>& users, const vector<RaceResult>& results, Leaderboard& leaderboard);

//Display functions
void displaySortingMenu(vector<Driver>& drivers, vector<Constructors>& constructors);
void displayAVLMenu(AVLTree& avlTree);
void displayLeaderboardMenu(Leaderboard& leaderboard, vector<User*>& loadedUsers);
void displayMainMenu();

int main() {
    srand(time(0));

    // Load data - points are now loaded from CSV
    vector<Driver> drivers = LoadDrivers("../data/drivers.csv");
    vector<Constructors> constructors = LoadConstructors("../data/constructors.csv");

    cout << "Loaded " << drivers.size() << " drivers with their points from CSV." << endl;

    // NEW: Load users from CSV
    vector<User*> loadedUsers = LoadUsersFromCSV("../data/users.csv", drivers, constructors);
    cout << "Loaded " << loadedUsers.size() << " users from CSV." << endl;

    // NEW: Create and populate AVL Tree
    AVLTree avlTree;

    for (Driver& d : drivers) {
        avlTree.insert(d);
    }


    // After building AVL tree, add:
    Trie driverTrie;

    for (Driver& d : drivers) {
        driverTrie.insert(&d);
    }


    // After building Trie, add:
    Leaderboard leaderboard;

    // NEW: Populate leaderboard with loaded users
    for (User* user : loadedUsers) {
        leaderboard.addUser(user);
    }



    // Main program loop
    while (true) {
        displayMainMenu();

        int choice;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {
            case 1: {
                // Create New Team
                string username;
                float budget;

                cout << "\n=== CREATE NEW TEAM ===" << endl;
                cout << "Enter username: ";
                getline(cin, username);

                cout << "Enter your budget (in millions): ";
                cin >> budget;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                Constructors chosenConstructor = SelectConstructor(constructors);
                Team userTeam(budget, chosenConstructor);

                int maxDrivers = 2;

                for (int i = 0; i < maxDrivers; ++i) {
                    Driver d = SelectDriver(drivers);
                    userTeam.addDriver(d);
                    cout << d.getName() << " added to your team!\n";
                }

                // Create new user and add to loaded users vector
                User* user = new User(username, budget, userTeam);
                loadedUsers.push_back(user);

                vector<Driver> teamDrivers = userTeam.getDrivers();

                cout << "\n=== YOUR TEAM ===" << endl;
                cout << "Username: " << username << endl;
                cout << "Budget: " << budget << "M" << endl;
                cout << "Constructor: " << chosenConstructor.getTeamName()
                     << " (" << chosenConstructor.getPrice() << "M)" << endl;
                cout << "Drivers: ";
                for (Driver& d : teamDrivers) {
                    cout << d.getName() << " (" << d.getPrice() << "M) ";
                }
                cout << endl;

                float teamCost = chosenConstructor.getPrice();
                for (Driver& d : teamDrivers) {
                    teamCost += d.getPrice();
                }
                cout << "Total Team Cost: " << teamCost << "M / " << budget << "M" << endl;

                StoreUserTeam("../data/users.csv", *user);

                leaderboard.addUser(user);
                cout << " Team added to leaderboard!" << endl;
                break;
            }
            case 2:
                // QuickSort Rankings (SIMPLIFIED)
                displaySortingMenu(drivers, constructors);
                break;
            case 3:
                // AVL Tree Rankings (SIMPLIFIED)
                displayAVLMenu(avlTree);
                break;
            case 4: {
                // Update driver points
                cout << "\n=== UPDATE DRIVER POINTS ===" << endl;

                string code;
                cout << "Enter driver code: ";
                getline(cin, code);

                // Find driver in vector
                bool found = false;
                for (Driver& d : drivers) {
                    if (d.getCode() == code) {
                        int points;
                        cout << "Current points: " << d.getPoints() << endl;
                        cout << "Enter points to add: ";
                        cin >> points;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        d.addpoints(points);
                        cout << "New points: " << d.getPoints() << endl;

                        // Update CSV file
                        if (UpdateDriversCSV("../data/drivers.csv", drivers)) {
                            cout << " Points updated in CSV file!" << endl;
                        } else {
                            cout << " Failed to update CSV file!" << endl;
                        }

                        // Rebuild AVL tree
                        cout << "Rebuilding AVL tree..." << endl;
                        avlTree.clear();
                        for (Driver& dr : drivers) {
                            avlTree.insert(dr);
                        }
                        cout << "AVL tree updated!" << endl;

                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "Driver not found!" << endl;
                }
                break;
            }
            case 5: {
                // Trie Search Menu
                int searchChoice;
                do {
                    cout << "\n=== DRIVER SEARCH (TRIE) ===" << endl;
                    cout << "1. Search by Name Prefix" << endl;
                    cout << "2. Search by Driver Code" << endl;
                    cout << "3. Search Exact Name" << endl;
                    cout << "4. Back to Main Menu" << endl;
                    cout << "Choose option: ";

                    cin >> searchChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    switch(searchChoice) {
                        case 1: {
                            string prefix;
                            cout << "Enter name prefix (e.g., 'Max', 'Lew'): ";
                            getline(cin, prefix);

                            vector<Driver*> results = driverTrie.searchDriversByPrefix(prefix);

                            if (results.empty()) {
                                cout << "No drivers found with prefix '" << prefix << "'" << endl;
                            } else {
                                cout << "\n=== SEARCH RESULTS ===" << endl;
                                cout << "Found " << results.size() << " driver(s):" << endl;
                                cout << "Code | Name                | Team           | Points | Price" << endl;
                                cout << "----------------------------------------------------------------" << endl;

                                for (Driver* d : results) {
                                    cout << d->getCode() << "  | "
                                         << setw(19) << left << d->getName() << " | "
                                         << setw(14) << d->getTeam() << " | "
                                         << setw(6) << right << d->getPoints() << " | "
                                         << d->getPrice() << "M" << endl;
                                }
                            }
                            break;
                        }

                        case 2: {
                            string code;
                            cout << "Enter driver code (e.g., 'VER', 'HAM'): ";
                            getline(cin, code);

                            Driver* result = nullptr;
                            for (Driver& d : drivers) {
                                if (d.getCode() == code) {
                                    result = &d;
                                    break;
                                }
                            }

                            if (result == nullptr) {
                                cout << "No driver found with code '" << code << "'" << endl;
                            } else {
                                cout << "\n=== DRIVER FOUND ===" << endl;
                                cout << "Code: " << result->getCode() << endl;
                                cout << "Name: " << result->getName() << endl;
                                cout << "Team: " << result->getTeam() << endl;
                                cout << "Points: " << result->getPoints() << endl;
                                cout << "Price: " << result->getPrice() << "M" << endl;
                            }
                            break;
                        }

                        case 3: {
                            string name;
                            cout << "Enter exact driver name: ";
                            getline(cin, name);

                            Driver* result = driverTrie.searchExact(name);

                            if (result == nullptr) {
                                cout << "No driver found with name '" << name << "'" << endl;
                            } else {
                                cout << "\n=== DRIVER FOUND ===" << endl;
                                cout << "Code: " << result->getCode() << endl;
                                cout << "Name: " << result->getName() << endl;
                                cout << "Team: " << result->getTeam() << endl;
                                cout << "Points: " << result->getPoints() << endl;
                                cout << "Price: " << result->getPrice() << "M" << endl;
                            }
                            break;
                        }

                        case 4:
                            cout << "Returning to main menu..." << endl;
                            break;

                        default:
                            cout << "Invalid choice!" << endl;
                    }

                    if (searchChoice != 4) {
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                } while (searchChoice != 4);
                break;
            }
            case 6: {
                // Race Simulation
                int raceChoice;
                do {
                    cout << "\n=== RACE SIMULATION ===" << endl;
                    cout << "1. Quick Race (All Drivers)" << endl;
                    cout << "2. Custom Race (Select Drivers)" << endl;
                    cout << "3. Season Simulation (Multiple Races)" << endl;
                    cout << "4. Back to Main Menu" << endl;
                    cout << "Choose option: ";

                    cin >> raceChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    switch(raceChoice) {
                        case 1: {
                            // Quick race with all drivers
                            string raceName;
                            cout << "Enter race name (e.g., Monaco GP): ";
                            getline(cin, raceName);

                            RaceSimulator race(raceName);

                            // Add all drivers
                            for (Driver& d : drivers) {
                                race.addDriver(&d);
                            }

                            // Run the race
                            race.runRace();
                            race.displayDetailedResults();

                            // Track performance for each driver
                            vector<RaceResult> results = race.getResults();

                            // NEW: Update user points based on their drivers' performance
                            cout << "\n Updating user points based on driver performance..." << endl;
                            int usersUpdated = 0;
                            for (User* user : loadedUsers) {
                                int userPointsGained = 0;
                                vector<Driver> userDrivers = user->getTeam().getDrivers();

                                // Check each of the user's drivers
                                for (const Driver& userDriver : userDrivers) {
                                    // Find this driver in the race results
                                    for (const RaceResult& result : results) {
                                        if (result.driver != nullptr &&
                                            result.driver->getName() == userDriver.getName()) {
                                            userPointsGained += result.pointsEarned;
                                            break;
                                            }
                                    }
                                }

                                if (userPointsGained > 0) {
                                    user->addPoints(userPointsGained);
                                    cout << "  " << user->getUsername() << ": +" << userPointsGained
                                         << " points (Total: " << user->getPoints() << ")" << endl;
                                    usersUpdated++;
                                }
                            }

                            if (usersUpdated > 0) {

                                // Update leaderboard
                                leaderboard.clear();
                                for (User* user : loadedUsers) {
                                    leaderboard.addUser(user);
                                }
                                cout << " Leaderboard updated!" << endl;

                                // Save to CSV
                                if (UpdateUsersCSV("../data/users.csv", loadedUsers)) {
                                    cout << " User points saved to CSV!" << endl;
                                }
                            } else {
                                cout << "ℹ  No users had drivers in this race" << endl;
                            }

                            // Update CSV after race
                            if (UpdateDriversCSV("../data/drivers.csv", drivers)) {
                                cout << " Driver points updated in CSV file!" << endl;
                            } else {
                                cout << " Failed to update CSV file!" << endl;
                            }

                            // Rebuild AVL tree with updated points
                            cout << "Rebuilding AVL tree with updated points..." << endl;
                            avlTree.clear();
                            for (Driver& dr : drivers) {
                                avlTree.insert(dr);
                            }
                            cout << "AVL tree updated!" << endl;


                            // Show top 3 users in leaderboard after race
                            cout << "\n TOP 3 USERS AFTER RACE:" << endl;
                            leaderboard.displayTopN(3);

                            break;
                        }

                        case 2: {
                            // Custom race
                            string raceName;
                            cout << "Enter race name: ";
                            getline(cin, raceName);

                            RaceSimulator race(raceName);

                            cout << "\nSelect drivers for the race:" << endl;
                            cout << "Available drivers:" << endl;
                            for (size_t i = 0; i < drivers.size(); i++) {
                                cout << i+1 << ". " << drivers[i].getName()
                                     << " (" << drivers[i].getTeam() << ")" << endl;
                            }

                            int numDrivers;
                            cout << "\nHow many drivers? (2-" << drivers.size() << "): ";
                            cin >> numDrivers;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            if (numDrivers < 2 || numDrivers > (int)drivers.size()) {
                                cout << "Invalid number of drivers!" << endl;
                                break;
                            }

                            cout << "Enter driver numbers separated by spaces: ";
                            for (int i = 0; i < numDrivers; i++) {
                                int idx;
                                cin >> idx;
                                if (idx >= 1 && idx <= (int)drivers.size()) {
                                    race.addDriver(&drivers[idx-1]);
                                }
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            race.runRace();
                            race.displayDetailedResults();

                            // Track performance for each driver
                            vector<RaceResult> results = race.getResults();

                            // Update user points based on their drivers' performance
                            cout << "\n Updating user points based on driver performance..." << endl;
                            int usersUpdated = 0;
                            for (User* user : loadedUsers) {
                                int userPointsGained = 0;
                                vector<Driver> userDrivers = user->getTeam().getDrivers();

                                // Check each of the user's drivers
                                for (const Driver& userDriver : userDrivers) {
                                    // Find this driver in the race results
                                    for (const RaceResult& result : results) {
                                        if (result.driver != nullptr &&
                                            result.driver->getName() == userDriver.getName()) {
                                            userPointsGained += result.pointsEarned;
                                            break;
                                            }
                                    }
                                }

                                if (userPointsGained > 0) {
                                    user->addPoints(userPointsGained);
                                    usersUpdated++;
                                }
                            }

                            if (usersUpdated > 0) {

                                // Update leaderboard
                                leaderboard.clear();
                                for (User* user : loadedUsers) {
                                    leaderboard.addUser(user);
                                }
                                cout << " Leaderboard updated!" << endl;

                                // Save to CSV
                                if (UpdateUsersCSV("../data/users.csv", loadedUsers)) {
                                    cout << " User points saved to CSV!" << endl;
                                }
                            } else {
                                cout << "ℹ  No users had drivers in this race" << endl;
                            }

                            // Update CSV after race
                            if (UpdateDriversCSV("../data/drivers.csv", drivers)) {
                                cout << " Driver points updated in CSV file!" << endl;
                            } else {
                                cout << " Failed to update CSV file!" << endl;
                            }

                            // Rebuild AVL tree
                            cout << "Rebuilding AVL tree with updated points..." << endl;
                            avlTree.clear();
                            for (Driver& dr : drivers) {
                                avlTree.insert(dr);
                            }
                            cout << "AVL tree updated!" << endl;

                            // Show top 3 users in leaderboard after race
                            cout << "\n TOP 3 USERS AFTER RACE:" << endl;
                            leaderboard.displayTopN(3);

                            break;
                        }

                        case 3: {
                            // Season simulation
                            int numRaces;
                            cout << "How many races in the season? (1-23): ";
                            cin >> numRaces;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            if (numRaces < 1 || numRaces > 23) {
                                cout << "Invalid number of races!" << endl;
                                break;
                            }

                            cout << "\n🏁 SIMULATING " << numRaces << " RACES..." << endl;

                            string raceNames[] = {"Bahrain", "Saudi Arabia", "Australia", "Japan", "China",
                                                "Miami", "Monaco", "Spain", "Canada", "Austria",
                                                "Britain", "Hungary", "Belgium", "Netherlands", "Italy",
                                                "Singapore", "USA", "Mexico", "Brazil", "Las Vegas",
                                                "Qatar", "Abu Dhabi", "Testing"};

                            for (int i = 0; i < numRaces; i++) {
                                string raceName = raceNames[i % 23] + " GP";
                                cout << "\n--- Race " << (i+1) << "/" << numRaces << ": " << raceName << " ---" << endl;

                                RaceSimulator race(raceName);
                                for (Driver& d : drivers) {
                                    race.addDriver(&d);
                                }

                                race.runRace();
                                race.displayResults();

                                // Track performance for each driver in this race
                                vector<RaceResult> results = race.getResults();

                                // NEW: Update user points based on their drivers' performance
                                for (User* user : loadedUsers) {
                                    int userPointsGained = 0;
                                    vector<Driver> userDrivers = user->getTeam().getDrivers();

                                    // Check each of the user's drivers
                                    for (const Driver& userDriver : userDrivers) {
                                        // Find this driver in the race results
                                        for (const RaceResult& result : results) {
                                            if (result.driver != nullptr &&
                                                result.driver->getName() == userDriver.getName()) {
                                                userPointsGained += result.pointsEarned;
                                                break;
                                                }
                                        }
                                    }

                                    if (userPointsGained > 0) {
                                        user->addPoints(userPointsGained);
                                    }
                                }

                                cout << "\nPress Enter for next race...";
                                cin.get();
                            }

                            cout << "\n All race performance data has been recorded!" << endl;

                            // Update CSV after all races
                            if (UpdateDriversCSV("../data/drivers.csv", drivers)) {
                                cout << " All driver points updated in CSV file!" << endl;
                            } else {
                                cout << " Failed to update CSV file!" << endl;
                            }

                            // Save user points to CSV
                            if (UpdateUsersCSV("../data/users.csv", loadedUsers)) {
                                cout << " User points saved to CSV!" << endl;
                            }

                            // Update leaderboard
                            leaderboard.clear();
                            for (User* user : loadedUsers) {
                                leaderboard.addUser(user);
                            }

                            // Final standings
                            cout << "\n FINAL SEASON STANDINGS " << endl;
                            avlTree.clear();
                            for (Driver& dr : drivers) {
                                avlTree.insert(dr);
                            }
                            avlTree.displayRankings();

                            // Show final user leaderboard
                            cout << "\n FINAL USER LEADERBOARD:" << endl;
                            leaderboard.displayTopN(10);

                            break;
                        }

                        case 4:
                            cout << "Returning to main menu..." << endl;
                            break;

                        default:
                            cout << "Invalid choice!" << endl;
                    }

                    if (raceChoice != 4 && raceChoice >= 1 && raceChoice <= 3) {
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                } while (raceChoice != 4);
                break;
            }
            case 7:
                // NEW: Leaderboard Menu
                displayLeaderboardMenu(leaderboard, loadedUsers);
                break;
            case 8:
                cout << "\n========================================" << endl;
                cout << "  Thanks for playing F1 Fantasy League!" << endl;
                cout << "========================================" << endl;
                cout << "\nSession Summary:" << endl;
                cout << "Teams Created: " << leaderboard.getTotalUsers() << endl;

                UpdateUsersCSV("../data/users.csv", loadedUsers);

                // Clean up dynamically allocated users
                for (User* user : loadedUsers) {
                    delete user;
                }

                cout << "\nGoodbye! " << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}

void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "        F1 FANTASY LEAGUE 2024" << endl;
    cout << "========================================" << endl;
    cout << "1. Create New Fantasy Team" << endl;
    cout << "2. Driver Rankings (QuickSort)" << endl;
    cout << "3. Driver Rankings (AVL Tree)" << endl;
    cout << "4. Update Driver Points" << endl;
    cout << "5. Search Drivers (Trie)" << endl;
    cout << "6. Simulate Race" << endl;
    cout << "7. Leaderboard" << endl;
    cout << "8. Exit" << endl;
    cout << "========================================" << endl;
}

// SIMPLIFIED AVL MENU (from 8 options to 4)
void displayAVLMenu(AVLTree& avlTree) {
    int choice;
    do {
        cout << "\n=== DRIVER RANKINGS - AVL TREE ===" << endl;
        cout << "1. View All Driver Rankings" << endl;
        cout << "2. View Top 10 Drivers" << endl;
        cout << "3. Search for Specific Driver" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {
            case 1: {
                cout << "\n=== ALL DRIVERS (AVL Tree - Sorted) ===" << endl;
                avlTree.displayRankings();
                break;
            }

            case 2: {
                vector<Driver> top10 = avlTree.getTopNDrivers(10);
                cout << "\n=== TOP 10 DRIVERS ===" << endl;
                cout << "Rank | Code | Name                | Points | Price" << endl;
                cout << "---------------------------------------------------" << endl;
                for (size_t i = 0; i < top10.size(); i++) {
                    cout << (i + 1) << ".   " << top10[i].getCode() << "     "
                         << setw(20) << left << top10[i].getName()
                         << setw(6) << right << top10[i].getPoints() << "     "
                         << top10[i].getPrice() << "M" << endl;
                }
                break;
            }

            case 3: {
                string code;
                cout << "Enter driver code: ";
                getline(cin, code);

                Driver* found = avlTree.search(code);
                if (found != nullptr) {
                    cout << "\n=== DRIVER FOUND ===" << endl;
                    cout << "Code: " << found->getCode() << endl;
                    cout << "Name: " << found->getName() << endl;
                    cout << "Team: " << found->getTeam() << endl;
                    cout << "Points: " << found->getPoints() << endl;
                    cout << "Price: " << found->getPrice() << "M" << endl;
                } else {
                    cout << "Driver not found!" << endl;
                }
                break;
            }

            case 4:
                cout << "Returning to main menu..." << endl;
                break;

            default:
                cout << "Invalid choice! Try again." << endl;
        }

        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 4);
}

// SIMPLIFIED SORTING MENU (from 8 options to 5)
void displaySortingMenu(vector<Driver>& drivers, vector<Constructors>& constructors) {
    int choice;
    do {
        cout << "\n=== DRIVER RANKINGS - QUICKSORT ===" << endl;
        cout << "1. Sort Drivers by Points (Highest First)" << endl;
        cout << "2. Sort Drivers by Price (Cheapest First)" << endl;
        cout << "3. View Top 10 by Points" << endl;
        cout << "4. Best Value Drivers (Points/Price)" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<Driver> sortedDrivers;

        switch(choice) {
            case 1: {
                sortedDrivers = drivers;
                SortingUtils::quickSortDriversByPoints(sortedDrivers);
                cout << "\n=== ALL DRIVERS SORTED BY POINTS ===" << endl;
                SortingUtils::displayTopDrivers(sortedDrivers);
                break;
            }

            case 2: {
                sortedDrivers = drivers;
                SortingUtils::quickSortDriversByPrice(sortedDrivers);
                cout << "\n=== ALL DRIVERS SORTED BY PRICE ===" << endl;
                SortingUtils::displayTopDrivers(sortedDrivers);
                break;
            }

            case 3: {
                sortedDrivers = drivers;
                SortingUtils::quickSortDriversByPoints(sortedDrivers);
                cout << "\n=== TOP 10 DRIVERS BY POINTS ===" << endl;
                SortingUtils::displayTopDrivers(sortedDrivers, 10);
                break;
            }

            case 4: {
                cout << "\n=== BEST VALUE DRIVERS ===" << endl;
                cout << "(High points relative to price)" << endl;

                vector<pair<float, Driver>> valueDrivers;
                for (Driver& d : drivers) {
                    if (d.getPrice() > 0) {
                        float valueScore = d.getPoints() / d.getPrice();
                        valueDrivers.push_back({valueScore, d});
                    }
                }

                sort(valueDrivers.begin(), valueDrivers.end(),
                     [](const pair<float, Driver>& a, const pair<float, Driver>& b) {
                         return a.first > b.first;
                     });

                cout << "\nRank | Driver\t\t\t| Points | Price | Value Score" << endl;
                cout << "--------------------------------------------------------" << endl;
                for (size_t i = 0; i < min((size_t)10, valueDrivers.size()); i++) {
                    cout << (i+1) << ".   " << setw(20) << left << valueDrivers[i].second.getName()
                         << setw(6) << right << valueDrivers[i].second.getPoints() << "   "
                         << setw(5) << valueDrivers[i].second.getPrice() << "M   "
                         << fixed << setprecision(2) << valueDrivers[i].first << endl;
                }
                break;
            }

            case 5:
                cout << "Returning to main menu..." << endl;
                break;

            default:
                cout << "Invalid choice! Try again." << endl;
        }

        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 5);
}

// NEW: LEADERBOARD MENU (with CSV support)
void displayLeaderboardMenu(Leaderboard& leaderboard, vector<User*>& loadedUsers) {
    int choice;
    do {
        cout << "\n=== LEADERBOARD ===" << endl;
        cout << "Total Players: " << leaderboard.getTotalUsers() << endl;
        cout << "Average Points: " << leaderboard.getAveragePoints() << endl;
        cout << "\n1. View Top 10 Players" << endl;
        cout << "2. View Full Leaderboard" << endl;
        cout << "3. Search Player Stats" << endl;
        cout << "4. Export Leaderboard" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Choose option: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {
            case 1: {
                leaderboard.displayTopN(10);
                break;
            }

            case 2: {
                leaderboard.displayFullLeaderboard();
                break;
            }

            case 3: {
                string username;
                cout << "Enter username: ";
                getline(cin, username);

                if (leaderboard.userExists(username)) {
                    leaderboard.displayUserStats(username);
                } else {
                    cout << "User '" << username << "' not found in leaderboard!" << endl;
                }
                break;
            }

            case 4: {
                string filename;
                cout << "Enter filename (e.g., leaderboard.txt): ";
                getline(cin, filename);

                if (leaderboard.exportToFile("../data/" + filename)) {
                    cout << " Leaderboard exported successfully!" << endl;
                } else {
                    cout << " Failed to export leaderboard!" << endl;
                }
                break;
            }

            case 5:
                cout << "Returning to main menu..." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
        }

        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 5);
}

// ============ HELPER FUNCTIONS ============

vector<Driver> LoadDrivers(string filename) {
    vector<Driver> drivers;
    ifstream file(filename);
    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string code, name, team;
        int points, avgFinishes, wins, podiums;
        float price;

        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, team, ',');
        ss >> points; ss.ignore();
        ss >> price; ss.ignore();
        ss >> avgFinishes; ss.ignore();
        ss >> wins; ss.ignore();
        ss >> podiums;

        drivers.push_back(Driver(code, name, team, points, price, avgFinishes, wins, podiums));
    }
    return drivers;
}

vector<Constructors> LoadConstructors(string filename) {
    vector<Constructors> constructors;
    ifstream file(filename);
    string line;
    getline(file,line);

    while (getline(file,line)) {
        stringstream ss(line);
        string code, teamname;
        float price;

        getline(ss,code,',');
        getline(ss,teamname, ',');
        ss >> price; ss.ignore();

        constructors.push_back(Constructors(code,teamname,price));
    }
    return constructors;
}

// NEW: Load users from CSV file
vector<User*> LoadUsersFromCSV(string filename, const vector<Driver>& drivers, const vector<Constructors>& constructors) {
    vector<User*> users;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Warning: Could not open " << filename << endl;
        return users;
    }

    string line;
    getline(file, line);  // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string username, constructorName, driver1Name, driver2Name;
        float budget;
        int points;

        getline(ss, username, ',');
        ss >> budget; ss.ignore();
        getline(ss, constructorName, ',');
        getline(ss, driver1Name, ',');
        getline(ss, driver2Name, ',');
        ss >> points;

        // Find constructor
        Constructors* foundConstructor = nullptr;
        for (const Constructors& c : constructors) {
            if (c.getTeamName() == constructorName) {
                foundConstructor = new Constructors(c);
                break;
            }
        }

        if (foundConstructor == nullptr) {
            // Use default constructor if not found
            foundConstructor = new Constructors("UNK", "Unknown", 10.0);
        }

        // Create team with constructor
        Team* userTeam = new Team(budget, *foundConstructor);

        // Find and add drivers
        for (const Driver& d : drivers) {
            if (d.getName() == driver1Name || d.getName() == driver2Name) {
                userTeam->addDriver(d);
            }
        }

        // Create user
        User* user = new User(username, budget, *userTeam);
        user->setPoints(points);  // Set loaded points

        users.push_back(user);

        delete foundConstructor;
        delete userTeam;
    }

    file.close();
    return users;
}

//Update users CSV file
bool UpdateUsersCSV(string filename, const vector<User*>& users) {
    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Write header
    file << "username,budget,constructor,driver_1,driver_2,points\n";

    // Write each user's data
    for (const User* user : users) {
        file << user->getUsername() << ","
             << user->getBudget() << ","
             << user->getTeam().getConstructor().getTeamName() << ",";

        vector<Driver> teamDrivers = user->getTeam().getDrivers();
        if (teamDrivers.size() >= 2) {
            file << teamDrivers[0].getName() << ","
                 << teamDrivers[1].getName() << ",";
        } else if (teamDrivers.size() == 1) {
            file << teamDrivers[0].getName() << ",None,";
        } else {
            file << "None,None,";
        }

        file << user->getPoints() << "\n";
    }

    file.close();
    return true;
}

void updateUserPointsFromRace(vector<User*>& users, const vector<RaceResult>& results, Leaderboard& leaderboard) {
    cout << "\nUpdating user points based on driver performance..." << endl;
    int usersUpdated = 0;

    for (User* user : users) {
        int userPointsGained = 0;
        vector<Driver> userDrivers = user->getTeam().getDrivers();

        // Check each of the user's drivers
        for (const Driver& userDriver : userDrivers) {
            // Find this driver in the race results
            for (const RaceResult& result : results) {
                if (result.driver != nullptr &&
                    result.driver->getName() == userDriver.getName()) {
                    userPointsGained += result.pointsEarned;
                    break;
                    }
            }
        }

        if (userPointsGained > 0) {
            user->addPoints(userPointsGained);
            cout << "  " << user->getUsername() << ": +" << userPointsGained
                 << " points (Total: " << user->getPoints() << ")" << endl;
            usersUpdated++;
        }
    }

    if (usersUpdated > 0) {
        cout << " Updated points for " << usersUpdated << " users!" << endl;

        // Update leaderboard
        leaderboard.clear();
        for (User* user : users) {
            leaderboard.addUser(user);
        }
        cout << " Leaderboard updated!" << endl;
    } else {
        cout << "ℹ  No users had drivers in this race" << endl;
    }
}



Driver SelectDriver(const vector<Driver> &drivers) {
    for ( Driver driver : drivers ) {
        cout << driver.getCode() << " " << driver.getName() << " " << driver.getTeam() << " " << driver.getPrice() << endl;
    }

    string cd;
    while (true) {
        cout << "Enter Driver code: ";
        getline(cin, cd);
        for (Driver drive : drivers) {
            if (cd == drive.getCode()) {
                return drive;
            }
        }
        cout << "Invalid Code" << endl;
    }
}

Constructors SelectConstructor(const vector<Constructors> &constructors) {
    for (Constructors c : constructors) {
        cout << c.getCode() << " " << c.getTeamName() << " " << c.getPrice() << endl;
    }

    string cd;
    while (true) {
        cout << "Enter Constructor code: ";
        getline(cin,cd);
        for (Constructors c : constructors) {
            if (cd == c.getCode()) {
                return c;
            }
        }
        cout << "Invalid Code" << endl;
    }
}

bool StoreUserTeam(string filename, User user) {
    ofstream file(filename, ios::app);

    file << user.getUsername() << "," << user.getBudget() << "," << user.getTeam().getConstructor().getTeamName() << ",";
    vector<Driver> drivers = user.getTeam().getDrivers();
    for (Driver d : drivers) {
        file << d.getName() << ",";
    }
    file << user.getPoints();
    file << "\n";
    return true;
}

// Function to update drivers CSV file with current points
bool UpdateDriversCSV(string filename, const vector<Driver>& drivers) {
    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Write header
    file << "code,name,team,points,price,avgFinishes,wins,podiums\n";

    // Write each driver's data
    for (const Driver& d : drivers) {
        file << d.getCode() << ","
             << d.getName() << ","
             << d.getTeam() << ","
             << d.getPoints() << ","
             << d.getPrice() << ","
             << "0,0,0\n";
    }

    file.close();
    return true;
}