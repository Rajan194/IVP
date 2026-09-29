#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

// ==========================================
// PROBLEM 1 & 2: CONTINUOUS OPTIMIZATION
// ==========================================

// Function for Problem 1: f(x) = -x^2 + 4x + 10 (Maximize)
double f1(double x) {
    return -(x * x) + (4 * x) + 10;
}

// Function for Problem 2: f(x) = x^2 + 10*sin(x) (Minimize)
double f2(double x) {
    return (x * x) + 10 * sin(x);
}

void solveContinuousHC() {
    cout << "--- 1. Hill Climbing (Maximize: -x^2 + 4x + 10) ---\n";
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10.0, 10.0);

    double current_x = dis(gen);
    double current_val = f1(current_x);
    double step_size = 0.01;
    
    while (true) {
        double left_x = current_x - step_size;
        double right_x = current_x + step_size;

        double left_val = f1(left_x);
        double right_val = f1(right_x);

        // Move to the strictly better neighbor (Steepest Ascent)
        if (left_val > current_val && left_val >= right_val) {
            current_x = left_x;
            current_val = left_val;
        } else if (right_val > current_val && right_val > left_val) {
            current_x = right_x;
            current_val = right_val;
        } else {
            break; // Peak reached
        }
    }
    cout << fixed << setprecision(4);
    cout << "Maximum found at x: " << current_x << " | f(x) = " << current_val << "\n\n";
}

void solveContinuousSA() {
    cout << "--- 2. Simulated Annealing (Minimize: x^2 + 10*sin(x)) ---\n";
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> start_dis(-10.0, 10.0);
    uniform_real_distribution<> step_dis(-1.0, 1.0);
    uniform_real_distribution<> prob_dis(0.0, 1.0);

    double current_x = start_dis(gen);
    double current_cost = f2(current_x);
    
    double best_x = current_x;
    double best_cost = current_cost;

    double temp = 100.0;
    double cooling_rate = 0.99;
    double min_temp = 1e-5;

    while (temp > min_temp) {
        double neighbor_x = current_x + step_dis(gen);
        double neighbor_cost = f2(neighbor_x);

        double cost_diff = neighbor_cost - current_cost;

        // Accept if better, OR probabilistically if worse
        if (cost_diff < 0 || prob_dis(gen) < exp(-cost_diff / temp)) {
            current_x = neighbor_x;
            current_cost = neighbor_cost;
            
            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_x = current_x;
            }
        }
        temp *= cooling_rate;
    }
    cout << "Global minimum found at x: " << best_x << " | f(x) = " << best_cost << "\n\n";
}


// ==========================================
// PROBLEM 3: TRAVELING SALESPERSON (TSP)
// ==========================================

int getTSPCost(const vector<int>& route, const vector<vector<int>>& dist) {
    int cost = 0, n = route.size();
    for (int i = 0; i < n - 1; ++i) cost += dist[route[i]][route[i+1]];
    cost += dist[route[n-1]][route[0]];
    return cost;
}

void solveTSP(const vector<vector<int>>& dist) {
    cout << "--- 3. Traveling Salesperson Problem (TSP) ---\n";
    int n = dist.size();
    vector<int> start_route(n);
    iota(start_route.begin(), start_route.end(), 0);
    
    random_device rd;
    mt19937 gen(rd());
    shuffle(start_route.begin() + 1, start_route.end(), gen);

    // --- Hill Climbing ---
    vector<int> hc_route = start_route;
    int hc_cost = getTSPCost(hc_route, dist);
    bool improvement = true;

    while (improvement) {
        improvement = false;
        int best_cost = hc_cost;
        vector<int> best_neighbor = hc_route;

        for (int i = 1; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                vector<int> neighbor = hc_route;
                swap(neighbor[i], neighbor[j]);
                int cost = getTSPCost(neighbor, dist);
                
                if (cost < best_cost) {
                    best_cost = cost;
                    best_neighbor = neighbor;
                    improvement = true;
                }
            }
        }
        if (improvement) {
            hc_route = best_neighbor;
            hc_cost = best_cost;
        }
    }
    cout << "[Hill Climbing]     Final Cost: " << hc_cost << "\n";

    // --- Simulated Annealing ---
    vector<int> sa_route = start_route;
    int sa_cost = getTSPCost(sa_route, dist);
    
    vector<int> best_sa_route = sa_route;
    int best_sa_cost = sa_cost;

    double temp = 10000.0;
    uniform_real_distribution<> prob_dist(0.0, 1.0);
    uniform_int_distribution<> node_dist(1, n - 1);

    while (temp > 1e-3) {
        int u = node_dist(gen), v = node_dist(gen);
        if (u == v) continue;

        vector<int> next_route = sa_route;
        swap(next_route[u], next_route[v]);
        int next_cost = getTSPCost(next_route, dist);

        if (next_cost < sa_cost || prob_dist(gen) < exp(-(next_cost - sa_cost) / temp)) {
            sa_route = next_route;
            sa_cost = next_cost;
            if (sa_cost < best_sa_cost) {
                best_sa_cost = sa_cost;
                best_sa_route = sa_route;
            }
        }
        temp *= 0.995;
    }
    cout << "[Simulated Anneal]  Final Cost: " << best_sa_cost << "\n\n";
}


// ==========================================
// PROBLEM 4: ARRAY PARTITIONING
// ==========================================

// Calculate cost: absolute difference between sum of Subset A and Subset B
long long getPartitionCost(const vector<int>& arr, const vector<bool>& in_subset_a) {
    long long sumA = 0, sumB = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (in_subset_a[i]) sumA += arr[i];
        else sumB += arr[i];
    }
    return abs(sumA - sumB);
}

void solveArrayPartition(const vector<int>& arr) {
    cout << "--- 4. Array Partitioning (Minimize Subset Difference) ---\n";
    int n = arr.size();
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> binary_dist(0, 1);
    uniform_int_distribution<> index_dist(0, n - 1);
    
    // Start with a random partition state
    vector<bool> start_state(n);
    for (int i = 0; i < n; ++i) start_state[i] = binary_dist(gen);
    
    // --- Hill Climbing ---
    vector<bool> hc_state = start_state;
    long long hc_cost = getPartitionCost(arr, hc_state);
    bool improvement = true;

    while (improvement) {
        improvement = false;
        long long best_cost = hc_cost;
        int best_flip_idx = -1;

        // Try flipping every single bit (element moves to the other subset)
        for (int i = 0; i < n; ++i) {
            hc_state[i] = !hc_state[i]; // Flip
            long long new_cost = getPartitionCost(arr, hc_state);
            
            if (new_cost < best_cost) {
                best_cost = new_cost;
                best_flip_idx = i;
                improvement = true;
            }
            hc_state[i] = !hc_state[i]; // Unflip to test next
        }
        
        if (improvement) {
            hc_state[best_flip_idx] = !hc_state[best_flip_idx]; // Commit the best flip
            hc_cost = best_cost;
        }
    }
    cout << "[Hill Climbing]     Min Diff: " << hc_cost << "\n";

    // --- Simulated Annealing ---
    vector<bool> sa_state = start_state;
    long long sa_cost = getPartitionCost(arr, sa_state);
    
    long long best_sa_cost = sa_cost;
    
    double temp = 100000.0;
    uniform_real_distribution<> prob_dist(0.0, 1.0);
    
    while (temp > 1e-4) {
        int idx_to_flip = index_dist(gen);
        
        sa_state[idx_to_flip] = !sa_state[idx_to_flip]; // Flip a random element
        long long next_cost = getPartitionCost(arr, sa_state);
        
        long long cost_diff = next_cost - sa_cost;
        
        if (cost_diff < 0 || prob_dist(gen) < exp(-cost_diff / temp)) {
            sa_cost = next_cost; // Accept
            if (sa_cost < best_sa_cost) {
                best_sa_cost = sa_cost;
            }
        } else {
            sa_state[idx_to_flip] = !sa_state[idx_to_flip]; // Reject (Unflip)
        }
        
        temp *= 0.999; // Slower cooling for finer search
    }
    cout << "[Simulated Anneal]  Min Diff: " << best_sa_cost << "\n";
}

// ==========================================
// MAIN DRIVER
// ==========================================

int main() {
    fast_io;

    // 1 & 2. Continuous
    solveContinuousHC();
    solveContinuousSA();

    // 3. TSP
    vector<vector<int>> tsp_dist = {
        {0, 29, 20, 21, 16, 31},
        {29, 0, 15, 17, 28, 18},
        {20, 15, 0, 28, 14, 22},
        {21, 17, 28, 0, 19, 12},
        {16, 28, 14, 19, 0, 14},
        {31, 18, 22, 12, 14, 0}
    };
    solveTSP(tsp_dist);

    // 4. Array Partitioning
    // A large array where DP subsets sum would be computationally heavy
    vector<int> large_array = {
        456, 123, 89, 234, 786, 112, 908, 444, 321, 678, 
        890, 56, 43, 222, 1098, 77, 88, 302, 604, 501,
        29, 999, 102, 47, 831, 350, 482, 10, 574, 915
    };
    solveArrayPartition(large_array);

    return 0;
}