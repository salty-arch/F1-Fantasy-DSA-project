//
// Created for Day 2: Sorting and Ranking
//
#ifndef F1_FANTASY_DSA_SORTINGUTILS_H
#define F1_FANTASY_DSA_SORTINGUTILS_H

#include <vector>
#include "../Core/Driver.h"
#include "../Core/Constructors.h"

using namespace std;

class SortingUtils {
public:

    // Quick Sort implementations
    static void quickSortDriversByPoints(vector<Driver>& drivers);
    static void quickSortDriversByPrice(vector<Driver>& drivers);
    static void quickSortDriversByAvgFinish(vector<Driver>& drivers);

    // Display functions
    static void displayTopDrivers(const vector<Driver>& drivers, int count = 10);
    static void displayDriversByPoints(const vector<Driver>& drivers);
    static void displayDriversByPrice(const vector<Driver>& drivers);
    static void displayDriversByAvgFinish(const vector<Driver>& drivers);

private:

    // Quick Sort helper functions
    static void quickSortByPoints(vector<Driver>& drivers, int low, int high);
    static int partitionByPoints(vector<Driver>& drivers, int low, int high);

    static void quickSortByPrice(vector<Driver>& drivers, int low, int high);
    static int partitionByPrice(vector<Driver>& drivers, int low, int high);
};

#endif //F1_FANTASY_DSA_SORTINGUTILS_H