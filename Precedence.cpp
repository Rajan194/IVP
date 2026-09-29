#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

const int NUM_THREADS = 5;

// Precedence: pairs of {Before, After}
vector<pair<int, int>> precedence = {
    {0, 2}, {1, 3}, {2, 4}, {0, 4}
};

// Calculate how many constraints a thread is involved in (Degree Heuristic)
int getDegree(int thread) {
    int degree = 0;
    for (auto edge : precedence) {
        if (edge.first == thread || edge.second == thread) degree++;
    }
    return degree;
}

// Check if assigning 'slot' to 'thread' violates any precedence rules
bool isValid(int thread, int slot, const vector<int>& assigned_slots) {
    // Check AllDiff constraint (no two threads in same slot)
    for (int t = 0; t < NUM_THREADS; t++) {
        if (assigned_slots[t] == slot) return false;
    }

    // Check Precedence constraint
    for (auto edge : precedence) {
        int before = edge.first;
        int after = edge.second;
        
        // If both are assigned, verify the 'before' thread finishes first
        if (assigned_slots[before] != 0 && assigned_slots[after] != 0) {
            if (assigned_slots[before] >= assigned_slots[after]) return false;
        }
        
        // If we are placing the 'after' thread, ensure its 'before' isn't placed later
        if (thread == after && assigned_slots[before] != 0 && assigned_slots[before] >= slot) return false;
        
        // If we are placing the 'before' thread, ensure its 'after' isn't placed earlier
        if (thread == before && assigned_slots[after] != 0 && assigned_slots[after] <= slot) return false;
    }
    return true;
}

// Select next thread using MRV, broken by Degree Heuristic
int selectNextThread(const vector<int>& assigned_slots) {
    int best_thread = -1;
    int min_domain_size = NUM_THREADS + 1;
    int max_degree = -1;

    for (int t = 0; t < NUM_THREADS; t++) {
        if (assigned_slots[t] != 0) continue; // Already scheduled

        int valid_slots = 0;
        for (int slot = 1; slot <= NUM_THREADS; slot++) {
            if (isValid(t, slot, assigned_slots)) valid_slots++;
        }

        int current_degree = getDegree(t);

        // Apply MRV
        if (valid_slots < min_domain_size) {
            min_domain_size = valid_slots;
            max_degree = current_degree;
            best_thread = t;
        } 
        // Apply Degree Heuristic Tie-Breaker
        else if (valid_slots == min_domain_size && current_degree > max_degree) {
            max_degree = current_degree;
            best_thread = t;
        }
    }
    return best_thread;
}

bool solveScheduling(int scheduled_count, vector<int>& assigned_slots) {
    if (scheduled_count == NUM_THREADS) return true;

    int thread = selectNextThread(assigned_slots);
    
    // Domain Wipeout
    if (thread == -1) return false;

    for (int slot = 1; slot <= NUM_THREADS; slot++) {
        if (isValid(thread, slot, assigned_slots)) {
            assigned_slots[thread] = slot; // Forward Assignment
            
            if (solveScheduling(scheduled_count + 1, assigned_slots)) {
                return true;
            }
            
            assigned_slots[thread] = 0; // Backtrack
        }
    }
    return false;
}

int main() {
    fast_io;
    vector<int> assigned_slots(NUM_THREADS, 0);

    if (solveScheduling(0, assigned_slots)) {
        cout << "Valid Thread Schedule:\n";
        vector<int> schedule(NUM_THREADS + 1);
        for (int t = 0; t < NUM_THREADS; t++) {
            schedule[assigned_slots[t]] = t;
        }
        for (int slot = 1; slot <= NUM_THREADS; slot++) {
            cout << "Time Slot " << slot << ": Thread T" << schedule[slot] << "\n";
        }
    } else {
        cout << "No valid schedule exists.\n";
    }
    return 0;
}