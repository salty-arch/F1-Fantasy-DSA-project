#include "Sorting.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ============ QUICK SORT IMPLEMENTATIONS ============

// Quick Sort by Points (Descending)
void SortingUtils::quickSortDriversByPoints(vector<Driver>& drivers) {
    if (drivers.empty()) return;
    quickSortByPoints(drivers, 0, drivers.size() - 1);
}

void SortingUtils::quickSortByPoints(vector<Driver>& drivers, int low, int high) {
    if (low < high) {
        int pi = partitionByPoints(drivers, low, high);
        quickSortByPoints(drivers, low, pi - 1);
        quickSortByPoints(drivers, pi + 1, high);
    }
}

// MODIFIED: Pivot is now the FIRST element
int SortingUtils::partitionByPoints(vector<Driver>& drivers, int low, int high) {
    // Choose FIRST element as pivot (was: drivers[high])
    int pivot = drivers[low].getPoints();
    int i = high + 1;  // Start from high+1 (reverse direction)

    for (int j = high; j > low; j--) {  // Changed: Iterate from high down to low+1
        // Descending order (>= instead of <=)
        if (drivers[j].getPoints() <= pivot) {  // <= because we're moving from right
            i--;
            swap(drivers[i], drivers[j]);
        }
    }

    // Place pivot in correct position
    swap(drivers[i - 1], drivers[low]);  // Changed: i-1 instead of i+1
    return i - 1;  // Changed: return i-1
}

// Quick Sort by Price (Ascending)
void SortingUtils::quickSortDriversByPrice(vector<Driver>& drivers) {
    if (drivers.empty()) return;
    quickSortByPrice(drivers, 0, drivers.size() - 1);
}

void SortingUtils::quickSortByPrice(vector<Driver>& drivers, int low, int high) {
    if (low < high) {
        int pi = partitionByPrice(drivers, low, high);
        quickSortByPrice(drivers, low, pi - 1);
        quickSortByPrice(drivers, pi + 1, high);
    }
}

// MODIFIED: Pivot is now the FIRST element
int SortingUtils::partitionByPrice(vector<Driver>& drivers, int low, int high) {
    // Choose FIRST element as pivot (was: drivers[high])
    float pivot = drivers[low].getPrice();
    int i = high + 1;  // Start from high+1

    for (int j = high; j > low; j--) {  // Iterate from high down to low+1
        // Ascending order
        if (drivers[j].getPrice() >= pivot) {  // >= because we're moving from right
            i--;
            swap(drivers[i], drivers[j]);
        }
    }

    // Place pivot in correct position
    swap(drivers[i - 1], drivers[low]);  // Changed: i-1 instead of i+1
    return i - 1;  // Changed: return i-1
}

// ============ DISPLAY FUNCTIONS ============

void SortingUtils::displayTopDrivers(const vector<Driver>& drivers, int count) {
    cout << "\n=== TOP " << min(count, (int)drivers.size()) << " DRIVERS ===" << endl;
    cout << "Rank | Code | Name                | Team           | Points | Price" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    int displayCount = min(count, (int)drivers.size());
    for (int i = 0; i < displayCount; i++) {
        cout << (i + 1) << ".   "
             << drivers[i].getCode() << "     "
             << drivers[i].getName();

        // Format name for consistent spacing
        int nameLength = drivers[i].getName().length();
        for (int j = nameLength; j < 20; j++) cout << " ";

        cout << drivers[i].getTeam();

        // Format team for consistent spacing
        int teamLength = drivers[i].getTeam().length();
        for (int j = teamLength; j < 15; j++) cout << " ";

        cout << drivers[i].getPoints() << "      "
             << drivers[i].getPrice() << "M" << endl;
    }
    cout << endl;
}

void SortingUtils::displayDriversByPoints(const vector<Driver>& drivers) {
    vector<Driver> sortedDrivers = drivers;
    quickSortDriversByPoints(sortedDrivers);
    displayTopDrivers(sortedDrivers);
}

void SortingUtils::displayDriversByPrice(const vector<Driver>& drivers) {
    vector<Driver> sortedDrivers = drivers;
    quickSortDriversByPrice(sortedDrivers);
    displayTopDrivers(sortedDrivers);
}