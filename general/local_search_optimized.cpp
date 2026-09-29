#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

struct State {
    std::vector<double> variables;
    double value; // Objective function value
    std::vector<State> getNeighbors() const; // Generates neighborhood
    State getRandomNeighbor() const;         // Picks one random neighbor
};

// Steepest-Ascent Hill Climbing
State SteepestAscentHillClimbing(State current) {
    while (true) {
        std::vector<State> neighbors = current.getNeighbors();
        State best_neighbor = current;
        
        for (const auto& neighbor : neighbors) {
            if (neighbor.value > best_neighbor.value) {
                best_neighbor = neighbor;
            }
        }
        
        if (best_neighbor.value <= current.value) {
            return current; // Local maximum reached
        }
        current = best_neighbor;
    }
}

// Stochastic Hill Climbing
State StochasticHillClimbing(State current, int max_iterations) {
    for (int i = 0; i < max_iterations; ++i) {
        std::vector<State> neighbors = current.getNeighbors();
        std::vector<State> better_neighbors;
        
        for (const auto& n : neighbors) {
            if (n.value > current.value) better_neighbors.push_back(n);
        }
        
        if (better_neighbors.empty()) return current;
        
        // Pick a random better neighbor
        int random_idx = rand() % better_neighbors.size();
        current = better_neighbors[random_idx];
    }
    return current;
}

// Simulated Annealing
State SimulatedAnnealing(State current, double initial_temp, double cooling_rate) {
    double T = initial_temp;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    while (T > 1e-5) {
        State next = current.getRandomNeighbor();
        double delta_E = next.value - current.value;
        
        if (delta_E > 0) {
            current = next;
        } else {
            double probability = std::exp(delta_E / T);
            if (dis(gen) < probability) {
                current = next;
            }
        }
        T *= cooling_rate; // Cool down
    }
    return current;
}

// Genetic Algorithm (Basic Skeleton)
State GeneticAlgorithm(std::vector<State> population, int generations) {
    for (int i = 0; i < generations; ++i) {
        std::vector<State> new_population;
        
        // Sort by fitness (descending)
        std::sort(population.begin(), population.end(), 
                 [](const State& a, const State& b) { return a.value > b.value; });
                 
        // Elitism & Crossover & Mutation logic goes here
        // ...
        
        population = new_population;
    }
    return population.front(); // Best found
}