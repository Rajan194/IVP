#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using Graph = std::unordered_map<int, std::vector<int>>;

bool BidirectionalSearch(const Graph& graph, int start, int goal) {
    if (start == goal) return true;

    std::queue<int> q_forward, q_backward;
    std::unordered_set<int> visited_forward, visited_backward;

    q_forward.push(start);
    visited_forward.insert(start);

    q_backward.push(goal);
    visited_backward.insert(goal);

    while (!q_forward.empty() && !q_backward.empty()) {
        // Expand forward
        int curr_f = q_forward.front();
        q_forward.pop();
        if (graph.find(curr_f) != graph.end()) {
            for (int neighbor : graph.at(curr_f)) {
                if (visited_backward.find(neighbor) != visited_backward.end()) return true;
                if (visited_forward.find(neighbor) == visited_forward.end()) {
                    visited_forward.insert(neighbor);
                    q_forward.push(neighbor);
                }
            }
        }

        // Expand backward
        int curr_b = q_backward.front();
        q_backward.pop();
        if (graph.find(curr_b) != graph.end()) {
            for (int neighbor : graph.at(curr_b)) {
                if (visited_forward.find(neighbor) != visited_forward.end()) return true;
                if (visited_backward.find(neighbor) == visited_backward.end()) {
                    visited_backward.insert(neighbor);
                    q_backward.push(neighbor);
                }
            }
        }
    }
    return false;
}

int main() {
    Graph g;
    // Graph representation: 1-2-3-4-5
    g[1] = {2}; g[2] = {1, 3}; g[3] = {2, 4}; g[4] = {3, 5}; g[5] = {4};
    
    int start = 1, goal = 5;
    bool found = BidirectionalSearch(g, start, goal);
    
    std::cout << "Path from " << start << " to " << goal;
    std::cout << (found ? " exists." : " does not exist.") << std::endl;
    
    return 0;
}