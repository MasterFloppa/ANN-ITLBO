#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

// Define parameters for the TSP
const int NUM_CITIES = 5;
const int MAX_ITERATIONS = 100;
const int POPULATION_SIZE = 20;

// Define a structure to represent a city
struct City
{
    double x, y;
};

// Define a structure to represent a route
struct Route
{
    vector<int> citySequence;
};

// Function to calculate the Euclidean distance between two cities
double calculateDistance(const City &city1, const City &city2)
{
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

// Function to calculate the total distance traveled for a route
double calculateTotalDistance(const Route &route, const vector<City> &cities)
{
    double totalDistance = 0.0;
    for (int i = 0; i < NUM_CITIES - 1; ++i)
    {
        int cityIndex1 = route.citySequence[i];
        int cityIndex2 = route.citySequence[i + 1];
        totalDistance += calculateDistance(cities[cityIndex1], cities[cityIndex2]);
    }
    // Add distance from last city back to the starting city
    totalDistance += calculateDistance(cities[route.citySequence[NUM_CITIES - 1]], cities[route.citySequence[0]]);
    return totalDistance;
}

// Function to initialize a random route

Route initializeRandomRoute()
{
    Route route;
    route.citySequence.resize(NUM_CITIES);
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        route.citySequence[i] = i;
    }
    random_shuffle(route.citySequence.begin(), route.citySequence.end());
    return route;
}

// Function to perform TLBO optimization
Route TLBO(const vector<City> &cities)
{
    // Initialize population
    vector<Route> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        population[i] = initializeRandomRoute();
    }

    // Main TLBO loop
    vector<double> fitness;
    for (int iter = 0; iter < MAX_ITERATIONS; ++iter)
    {
        // Calculate fitness for each route
        fitness = vector<double>(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; ++i)
        {
            fitness[i] = calculateTotalDistance(population[i], cities);
        }

        // Find the best and worst routes
        int bestIndex = min_element(fitness.begin(), fitness.end()) - fitness.begin();
        int worstIndex = max_element(fitness.begin(), fitness.end()) - fitness.begin();

        // Teaching phase
        for (int i = 0; i < POPULATION_SIZE; ++i)
        {
            if (i != bestIndex)
            {
                population[i] = population[bestIndex];
            }
        }

        // Learning phase
        for (int i = 0; i < POPULATION_SIZE; ++i)
        {
            if (i != worstIndex)
            {
                int swapIndex1 = rand() % NUM_CITIES;
                int swapIndex2 = rand() % NUM_CITIES;
                swap(population[i].citySequence[swapIndex1], population[i].citySequence[swapIndex2]);
            }
        }
    }

    // Return the best route
    int bestIndex = min_element(fitness.begin(), fitness.end()) - fitness.begin();
    return population[bestIndex];
}

int main()
{
    // Initialize random seed
    srand(time(nullptr));

    // Define cities
    vector<City> cities = {
        {0, 0}, // City A
        {1, 2}, // City B
        {3, 1}, // City C
        {2, 3}, // City D
        {4, 4}  // City E
    };

    // Perform TLBO optimization
    Route bestRoute = TLBO(cities);

    // Print the best route and its total distance
    cout << "Best Route: ";
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        cout << bestRoute.citySequence[i] << " ";
    }
    cout << endl;
    cout << "Total Distance: " << calculateTotalDistance(bestRoute, cities) << endl;

    return 0;
}
