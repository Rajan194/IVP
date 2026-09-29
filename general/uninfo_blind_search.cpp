#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using Graph = std::unordered_map<int, std::vector<int>>;

// Breadth-First Search (BFS)
bool BFS(const Graph& graph, int start, int goal) {
    std::queue<int> q;
    std::unordered_set<int> visited;
    
    q.push(start);
    visited.insert(start);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        if (current == goal) return true;
        
        for (int neighbor : graph.at(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return false;
}

// Depth-First Search (DFS) - Recursive
bool DFS_helper(const Graph& graph, int current, int goal, std::unordered_set<int>& visited) {
    if (current == goal) return true;
    visited.insert(current);
    
    for (int neighbor : graph.at(current)) {
        if (visited.find(neighbor) == visited.end()) {
            if (DFS_helper(graph, neighbor, goal, visited)) return true;
        }
    }
    return false;
}

bool DFS(const Graph& graph, int start, int goal) {
    std::unordered_set<int> visited;
    return DFS_helper(graph, start, goal, visited);
}

// Depth-Limited Search (DLS)
bool DLS(const Graph& graph, int current, int goal, int depth_limit) {
    if (current == goal) return true;
    if (depth_limit <= 0) return false;
    
    for (int neighbor : graph.at(current)) {
        if (DLS(graph, neighbor, goal, depth_limit - 1)) return true;
    }
    return false;
}

// Iterative Deepening Search (IDS)
bool IDS(const Graph& graph, int start, int goal, int max_depth) {
    for (int depth = 0; depth <= max_depth; ++depth) {
        if (DLS(graph, start, goal, depth)) return true;
    }
    return false;
}