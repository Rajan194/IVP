#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Fast I/O
#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

// Define Initial and Final (Goal) configurations from the problem statement
const string START = "123#46758"; // Represents {{1,2,3},{#,4,6},{7,5,8}}
const string GOAL  = "12345678#"; // Represents {{1,2,3},{4,5,6},{7,8,#}}

// Helper function to generate valid next states by sliding tiles into the empty space '#'
vector<string> getNeighbors(const string& state) {
    vector<string> neighbors;
    int idx = state.find('#');
    int r = idx / 3, c = idx % 3;

    // Directions: Up, Down, Left, Right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i], nc = c + dc[i];
        if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
            int n_idx = nr * 3 + nc;
            string next_state = state;
            swap(next_state[idx], next_state[n_idx]);
            neighbors.push_back(next_state);
        }
    }
    return neighbors;
}

// Helper to trace and print the number of moves
void printPath(const string& end_state, unordered_map<string, string>& parent) {
    int steps = 0;
    string curr = end_state;
    while (curr != "" && parent.count(curr)) {
        curr = parent[curr];
        if(curr != "") steps++;
    }
    cout << "Total moves to reach goal: " << steps << "\n";
}

// a) BFS Implementation
void solveBFS() {
    queue<string> q;
    unordered_set<string> visited;
    unordered_map<string, string> parent;

    q.push(START);
    visited.insert(START);
    parent[START] = "";

    while (!q.empty()) {
        string curr = q.front();
        q.pop();

        if (curr == GOAL) {
            cout << "BFS Solution Found!\n";
            printPath(curr, parent);
            return;
        }

        for (const string& next_state : getNeighbors(curr)) {
            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                parent[next_state] = curr;
                q.push(next_state);
            }
        }
    }
}

// b) DFS Implementation
void solveDFS() {
    stack<string> s;
    unordered_set<string> visited;
    unordered_map<string, string> parent;

    s.push(START);
    visited.insert(START);
    parent[START] = "";

    while (!s.empty()) {
        string curr = s.top();
        s.pop();

        if (curr == GOAL) {
            cout << "DFS Solution Found! (Note: Path is likely not optimal)\n";
            printPath(curr, parent);
            return;
        }

        for (const string& next_state : getNeighbors(curr)) {
            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                parent[next_state] = curr;
                s.push(next_state);
            }
        }
    }
}

// h(n) = Number of mismatched cells (excluding the empty space)
int getMismatches(const string& state) {
    int count = 0;
    for(int i = 0; i < 9; i++) {
        if(state[i] != '#' && state[i] != GOAL[i]) {
            count++;
        }
    }
    return count;
}

// Node structure for A* priority queue
struct AStarNode {
    int f, g;
    string state;
    bool operator>(const AStarNode& other) const {
        return f > other.f; 
    }
};

// c) A* Algorithm Implementation
void solveAStar() {
    // Min-heap based on f(n) value
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;
    unordered_map<string, int> best_g;
    unordered_map<string, string> parent;

    // g(n) = level of the tree node, h(n) = mismatched cells
    int initial_h = getMismatches(START);
    pq.push({initial_h, 0, START}); 
    best_g[START] = 0;
    parent[START] = "";

    while (!pq.empty()) {
        AStarNode curr = pq.top();
        pq.pop();

        if (curr.state == GOAL) {
            cout << "A* Solution Found!\n";
            printPath(curr.state, parent);
            return;
        }

        // Optimization: Skip if we've already found a better path to this state
        if (curr.g > best_g[curr.state]) continue;

        for (const string& next_state : getNeighbors(curr.state)) {
            int new_g = curr.g + 1; // g(n) increases by 1 for each tree level
            
            if (best_g.find(next_state) == best_g.end() || new_g < best_g[next_state]) {
                best_g[next_state] = new_g;
                parent[next_state] = curr.state;
                
                int h = getMismatches(next_state); // h(n)
                int f = new_g + h;                 // f(n) = g(n) + h(n)
                
                pq.push({f, new_g, next_state});
            }
        }
    }
}

int main() {
    fast_io;
    
    cout << "--- 8-Puzzle Solver ---\n\n";
    
    solveBFS();
    cout << "-----------------------\n";
    
    solveDFS();
    cout << "-----------------------\n";
    
    solveAStar();
    cout << "-----------------------\n";

    return 0;
}