#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Variables: 0=A, 1=B, 2=C, 3=D, 4=E
const int N = 5;
vector<string> var_names = {"A", "B", "C", "D", "E"};
vector<string> color_names = {"Unassigned", "Red", "Green", "Blue"};

// Neighbors: A(B,C), B(A,C,D,E), C(A,B,E), D(B,E), E(B,C,D)
vector<vector<int>> adj = {
    {1, 2},          // A
    {0, 2, 3, 4},    // B
    {0, 1, 4},       // C
    {1, 4},          // D
    {1, 2, 3}        // E
};

bool isValid(int u, int color, const vector<int>& assigned) {
    for (int v : adj[u]) {
        if (assigned[v] == color) return false;
    }
    return true;
}

// 2. Standard Backtracking Search
bool solveStandard(int u, vector<int>& assigned, int& backtracks, vector<string>& sequence) {
    if (u == N) return true; // All assigned

    for (int c = 1; c <= 3; ++c) {
        sequence.push_back("Trying " + var_names[u] + " = " + color_names[c]);
        
        if (isValid(u, c, assigned)) {
            assigned[u] = c;
            if (solveStandard(u + 1, assigned, backtracks, sequence)) return true;
            assigned[u] = 0; // Backtrack
        }
    }
    
    sequence.push_back("Backtracking at " + var_names[u]);
    backtracks++;
    return false;
}

// 5. MRV Heuristic
int getMRVNode(const vector<int>& assigned) {
    int min_rem = 10, best_u = -1;
    for (int i = 0; i < N; ++i) {
        if (assigned[i] != 0) continue;
        
        int valid_colors = 0;
        for (int c = 1; c <= 3; ++c) {
            if (isValid(i, c, assigned)) valid_colors++;
        }
        
        // Degree heuristic as tie-breaker could be added here, 
        // but simple MRV selects first lowest
        if (valid_colors < min_rem) {
            min_rem = valid_colors;
            best_u = i;
        }
    }
    return best_u;
}

bool solveMRV(int count, vector<int>& assigned, int& backtracks, vector<string>& sequence) {
    if (count == N) return true;

    int u = getMRVNode(assigned);
    
    for (int c = 1; c <= 3; ++c) {
        sequence.push_back("MRV chose " + var_names[u] + " -> Trying " + color_names[c]);
        
        if (isValid(u, c, assigned)) {
            assigned[u] = c;
            if (solveMRV(count + 1, assigned, backtracks, sequence)) return true;
            assigned[u] = 0;
        }
    }
    
    sequence.push_back("Backtracking at " + var_names[u]);
    backtracks++;
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> standard_assigned(N, 0);
    int standard_backtracks = 0;
    vector<string> standard_seq;
    
    cout << "--- Standard Backtracking ---\n";
    if (solveStandard(0, standard_assigned, standard_backtracks, standard_seq)) {
        for (size_t i = 0; i < standard_seq.size(); ++i) cout << standard_seq[i] << "\n";
        cout << "Standard Backtracks: " << standard_backtracks << "\n";
        cout << "Final Assignment: ";
        for(int i=0; i<N; i++) cout << var_names[i] << "=" << color_names[standard_assigned[i]] << " ";
        cout << "\n\n";
    }

    vector<int> mrv_assigned(N, 0);
    int mrv_backtracks = 0;
    vector<string> mrv_seq;
    
    cout << "--- MRV Backtracking ---\n";
    if (solveMRV(0, mrv_assigned, mrv_backtracks, mrv_seq)) {
        for (size_t i = 0; i < mrv_seq.size(); ++i) cout << mrv_seq[i] << "\n";
        cout << "MRV Backtracks: " << mrv_backtracks << "\n";
        cout << "Final Assignment: ";
        for(int i=0; i<N; i++) cout << var_names[i] << "=" << color_names[mrv_assigned[i]] << " ";
        cout << "\n";
    }
    return 0;
}