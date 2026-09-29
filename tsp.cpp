#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

// Fast I/O macro
#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

// Function to calculate the total cost of a given route
int calculateCost(const vector<int>& route, const vector<vector<int>>& dist) {
    int cost = 0;
    int n = route.size();
    for (int i = 0; i < n - 1; ++i) {
        cost += dist[route[i]][route[i+1]];
    }
    cost += dist[route[n-1]][route[0]]; // Return to start
    return cost;
}

// Function to print the route
void printRoute(const vector<int>& route, int cost) {
    for (int node : route) cout << node << " -> ";
    cout << route[0] << " | Total Cost: " << cost << "\n";
}

// 1. Steepest-Descent Hill Climbing
void solveHillClimbing(const vector<vector<int>>& dist) {
    int n = dist.size();
    vector<int> current_route(n);
    iota(current_route.begin(), current_route.end(), 0);
    
    random_device rd;
    mt19937 gen(rd());
    shuffle(current_route.begin() + 1, current_route.end(), gen); // Keep 0 as start

    int current_cost = calculateCost(current_route, dist);
    
    cout << "--- Hill Climbing ---\n";
    cout << "Initial Route: ";
    printRoute(current_route, current_cost);

    bool improvement = true;
    while (improvement) {
        improvement = false;
        int best_neighbor_cost = current_cost;
        vector<int> best_neighbor = current_route;

        // Generate all 2-opt neighbors and find the strictly best one
        for (int i = 1; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                vector<int> neighbor = current_route;
                swap(neighbor[i], neighbor[j]);
                
                int neighbor_cost = calculateCost(neighbor, dist);
                if (neighbor_cost < best_neighbor_cost) {
                    best_neighbor_cost = neighbor_cost;
                    best_neighbor = neighbor;
                    improvement = true;
                }
            }
        }

        if (improvement) {
            current_route = best_neighbor;
            current_cost = best_neighbor_cost;
        }
    }

    cout << "Final Route:   ";
    printRoute(current_route, current_cost);
    cout << "\n";
}

// 2. Simulated Annealing
void solveSimulatedAnnealing(const vector<vector<int>>& dist) {
    int n = dist.size();
    vector<int> current_route(n);
    iota(current_route.begin(), current_route.end(), 0);
    
    random_device rd;
    mt19937 gen(rd());
    shuffle(current_route.begin() + 1, current_route.end(), gen);

    int current_cost = calculateCost(current_route, dist);
    
    // Keep track of the absolute best path found during the entire process
    vector<int> best_overall_route = current_route;
    int best_overall_cost = current_cost;

    // Annealing parameters
    double temp = 10000.0;
    double cooling_rate = 0.995;
    double min_temp = 1e-3;
    
    uniform_real_distribution<> prob_dist(0.0, 1.0);
    uniform_int_distribution<> node_dist(1, n - 1);

    cout << "--- Simulated Annealing ---\n";
    cout << "Initial Route: ";
    printRoute(current_route, current_cost);

    while (temp > min_temp) {
        // Generate ONE random neighbor by swapping two random nodes
        int u = node_dist(gen);
        int v = node_dist(gen);
        while (u == v) v = node_dist(gen);

        vector<int> next_route = current_route;
        swap(next_route[u], next_route[v]);
        int next_cost = calculateCost(next_route, dist);

        // Calculate cost difference (positive means the new route is worse)
        int cost_diff = next_cost - current_cost;

        // Accept if it's better, OR probabilistically accept if it's worse
        if (cost_diff < 0 || prob_dist(gen) < exp(-cost_diff / temp)) {
            current_route = next_route;
            current_cost = next_cost;
            
            if (current_cost < best_overall_cost) {
                best_overall_cost = current_cost;
                best_overall_route = current_route;
            }
        }
        
        // Exponential cooling
        temp *= cooling_rate;
    }

    cout << "Final Route:   ";
    printRoute(best_overall_route, best_overall_cost);
    cout << "\n";
}

int main() {
    fast_io;

    // 6x6 Distance Matrix (0 is the hub, 1-5 are warehouses)
    vector<vector<int>> dist_matrix = {
        {0,  29, 20, 21, 16, 31},
        {29, 0,  15, 17, 28, 18},
        {20, 15, 0,  28, 14, 22},
        {21, 17, 28, 0,  19, 12},
        {16, 28, 14, 19, 0,  14},
        {31, 18, 22, 12, 14, 0}
    };

    solveHillClimbing(dist_matrix);
    solveSimulatedAnnealing(dist_matrix);

    return 0;
}
// input is distance matrix