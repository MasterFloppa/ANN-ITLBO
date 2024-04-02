#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// NOT WORKING CHANGES MADE ARE NOT SAVED!!!

const int N = 17;        // Population size
const int max_iter = 10; // Maximum iterations
const int dim = 9;       // Total dimensions

// Define upper and lower bounds for each dimension
const double lower_bound = -1e9;
const double upper_bound = 1e9;

// Initialize population with random positions
vector<vector<double>> initializePopulation()
{
    vector<vector<double>> population = {
        {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9},
        {-0.5, 0.6, -0.7, 0.8, -0.9, 1.0, -1.1, 1.2, -1.3},
        {0.3, -0.4, 0.5, -0.6, 0.7, -0.8, 0.9, -1.0, 1.1},
        {1.0, -0.9, 0.8, -0.7, 0.6, -0.5, 0.4, -0.3, 0.2},
        {-0.2, 0.3, -0.4, 0.5, -0.6, 0.7, -0.8, 0.9, -1.0},
        {0.7, -0.6, 0.5, -0.4, 0.3, -0.2, 0.1, -0.1, 0.0},
        {-1.0, 1.1, -1.2, 1.3, -1.4, 1.5, -1.6, 1.7, -1.8},
        {0.4, -0.5, 0.6, -0.7, 0.8, -0.9, 1.0, -1.1, 1.2},
        {-0.6, 0.7, -0.8, 0.9, -1.0, 1.1, -1.2, 1.3, -1.4},
        {0.8, -0.7, 0.6, -0.5, 0.4, -0.3, 0.2, -0.1, 0.0},
        {-1.2, 1.3, -1.4, 1.5, -1.6, 1.7, -1.8, 1.9, -2.0},
        {0.9, -1.0, 1.1, -1.2, 1.3, -1.4, 1.5, -1.6, 1.7},
        {-1.0, 0.9, -0.8, 0.7, -0.6, 0.5, -0.4, 0.3, -0.2},
        {1.3, -1.4, 1.5, -1.6, 1.7, -1.8, 1.9, -2.0, 2.1},
        {0.5, -0.4, 0.3, -0.2, 0.1, -0.1, 0.0, -0.1, 0.2},
        {-0.8, 0.9, -1.0, 1.1, -1.2, 1.3, -1.4, 1.5, -1.6},
        {1.2, -1.3, 1.4, -1.5, 1.6, -1.7, 1.8, -1.9, 2.0}};
    return population;
}

// Evaluate fitness of an individual (customize according to your objective function)
double evaluateFitness(const vector<double> &position)
{
    // Example fitness evaluation (replace with your own)
    double accuracy = nn.evaluate(position);
    // Convert accuracy to negative value for minimization
    return -accuracy;
}

// Perform TLBO optimization
vector<double> tlbo()
{
    vector<vector<double>> population = initializePopulation();
    srand(time(0)); // Seed random number generator
    for (int iter = 0; iter < max_iter; ++iter)
    {
        // Evaluate fitness of each individual
        vector<double> fitness(N);
        for (int i = 0; i < N; ++i)
        {
            fitness[i] = evaluateFitness(population[i]);
        }

        // Sort population based on fitness
        vector<int> indices(N);
        iota(indices.begin(), indices.end(), 0); // Fill indices with 0, 1, ..., N-1
        sort(indices.begin(), indices.end(), [&fitness](int a, int b)
             { return fitness[a] < fitness[b]; });

        // Teaching phase
        double mean_fitness = 0.0;
        for (int i = 0; i < N / 2; ++i)
        {
            mean_fitness += fitness[indices[i]];
        }
        mean_fitness /= N / 2;
        for (int i = 0; i < N / 2; ++i)
        {
            int idx = indices[i];
            for (int j = 0; j < dim; ++j)
            {
                population[idx][j] += (population[idx][j] - population[indices[N / 2 + i]][j]) * (fitness[idx] - mean_fitness) / fitness[idx];
            }
        }

        // Learning phase
        for (int i = 0; i < N; ++i)
        {
            int rand_index = rand() % N;
            if (rand_index == i)
            {
                rand_index = (rand_index + 1) % N;
            }
            for (int j = 0; j < dim; ++j)
            {
                population[i][j] += (population[indices[rand_index]][j] - population[i][j]) * (rand() / static_cast<double>(RAND_MAX));
            }
        }
    }

    // Evaluate fitness of final population
    vector<double> fitness(N);
    for (int i = 0; i < N; ++i)
    {
        fitness[i] = evaluateFitness(population[i]);
    }

    // Select the best individual as the result
    auto min_fitness = min_element(fitness.begin(), fitness.end());
    int best_index = distance(fitness.begin(), min_fitness);

    return population[best_index];
}

int main()
{
    int input_size = 2;
    int hidden_size = 3;
    int output_size = 1;
    NeuralNetwork nn1(input_size, hidden_size, output_size);
    nn = nn1;
    vector<double> solution = tlbo();
    // Print the best solution found
    cout << "Best solution found: ";
    for (double x : solution)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "fitness: " << -evaluateFitness(solution) << endl;

    return 0;
}
