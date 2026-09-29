#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

using Edge = std::pair<int, double>; // {neighbor, cost}
using WeightedGraph = std::unordered_map<int, std::vector<Edge>>;

// Dummy heuristic function
double heuristic(int node, int goal) {
    return 0.0; // Replace with actual heuristic calculation
}

// Dijkstra’s Algorithm (Uniform Cost Search)
double Dijkstra(const WeightedGraph& graph, int start, int goal) {
    using NodeCost = std::pair<double, int>; // {cumulative_cost, node}
    std::priority_queue<NodeCost, std::vector<NodeCost>, std::greater<NodeCost>> pq;
    std::unordered_map<int, double> costs;
    
    pq.push({0.0, start});
    costs[start] = 0.0;
    
    while (!pq.empty()) {
        double current_cost = pq.top().first;
        int current = pq.top().second;
        pq.pop();
        
        if (current == goal) return current_cost;
        if (current_cost > costs[current]) continue;
        
        for (const auto& edge : graph.at(current)) {
            int neighbor = edge.first;
            double new_cost = current_cost + edge.second;
            
            if (costs.find(neighbor) == costs.end() || new_cost < costs[neighbor]) {
                costs[neighbor] = new_cost;
                pq.push({new_cost, neighbor});
            }
        }
    }
    return -1.0; // Unreachable
}

// A* Search
double AStar(const WeightedGraph& graph, int start, int goal) {
    using NodeCost = std::pair<double, int>; // {f_score, node}
    std::priority_queue<NodeCost, std::vector<NodeCost>, std::greater<NodeCost>> pq;
    std::unordered_map<int, double> g_score;
    
    pq.push({heuristic(start, goal), start});
    g_score[start] = 0.0;
    
    while (!pq.empty()) {
        double f = pq.top().first;
        int current = pq.top().second;
        pq.pop();
        
        if (current == goal) return g_score[current];
        
        for (const auto& edge : graph.at(current)) {
            int neighbor = edge.first;
            double tentative_g = g_score[current] + edge.second;
            
            if (g_score.find(neighbor) == g_score.end() || tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                double f_score = tentative_g + heuristic(neighbor, goal);
                pq.push({f_score, neighbor});
            }
        }
    }
    return -1.0;
}

// Greedy Best-First Search
bool GreedyBFS(const WeightedGraph& graph, int start, int goal) {
    using NodeCost = std::pair<double, int>; // {heuristic_value, node}
    std::priority_queue<NodeCost, std::vector<NodeCost>, std::greater<NodeCost>> pq;
    std::unordered_set<int> visited;
    
    pq.push({heuristic(start, goal), start});
    visited.insert(start);
    
    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();
        
        if (current == goal) return true;
        
        for (const auto& edge : graph.at(current)) {
            int neighbor = edge.first;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                pq.push({heuristic(neighbor, goal), neighbor});
            }
        }
    }
    return false;
}