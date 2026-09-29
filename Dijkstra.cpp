#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Helper function to reconstruct and print the path
void printPath(const vector<int>& parent, int target) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    
    for (size_t i = 0; i < path.size(); ++i) {
        cout << (char)(path[i] + 'a');
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

// Dijkstra's Algorithm
void dijkstra(int start, int target, const vector<vector<pair<int, int>>>& adj) {
    int n = adj.size();
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    
    // Min-heap priority queue: {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        // C++11/14 compatible pair extraction
        pair<int, int> topNode = pq.top();
        int d = topNode.first;
        int u = topNode.second;
        pq.pop();
        
        if (d > dist[u]) continue;
        if (u == target) break; // Stop early if target is reached
        
        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    cout << "Dijkstra's Algorithm:\n";
    cout << "Shortest Path Cost: " << dist[target] << "\n";
    cout << "Path: ";
    printPath(parent, target);
    cout << "---------------------------\n";
}

// A* Algorithm
void aStar(int start, int target, const vector<vector<pair<int, int>>>& adj, const vector<int>& h) {
    int n = adj.size();
    vector<int> g_score(n, INF);
    vector<int> parent(n, -1);
    
    // Min-heap priority queue: {f_score, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    g_score[start] = 0;
    pq.push({h[start], start}); // f_score = g_score + h
    
    while (!pq.empty()) {
        // C++11/14 compatible pair extraction
        pair<int, int> topNode = pq.top();
        int f = topNode.first;
        int u = topNode.second;
        pq.pop();
        
        if (u == target) break; // Reached target
        
        if (f > g_score[u] + h[u]) continue;
        
        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            int tentative_g = g_score[u] + weight;
            
            if (tentative_g < g_score[v]) {
                parent[v] = u;
                g_score[v] = tentative_g;
                int f_score = g_score[v] + h[v];
                pq.push({f_score, v});
            }
        }
    }
    
    cout << "A* Algorithm:\n";
    cout << "Shortest Path Cost: " << g_score[target] << "\n";
    cout << "Path: ";
    printPath(parent, target);
    cout << "---------------------------\n";
}

int main() {
    // Fast I/O for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 6 nodes: a=0, b=1, c=2, d=3, e=4, f=5
    int n = 6;
    vector<vector<pair<int, int>>> adj(n);
    
    // Helper lambda to add undirected edges
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };
    
    // Graph edges from the problem statement
    addEdge(0, 1, 5); // a - b
    addEdge(0, 2, 2); // a - c
    addEdge(0, 3, 3); // a - d
    addEdge(1, 2, 2); // b - c
    addEdge(1, 5, 3); // b - f
    addEdge(2, 3, 1); // c - d
    addEdge(2, 4, 2); // c - e
    addEdge(2, 5, 6); // c - f
    addEdge(3, 4, 4); // d - e
    addEdge(4, 5, 4); // e - f
    
    // Heuristic values from the table (h[node])
    vector<int> h = {6, 2, 5, 6, 4, 0};
    
    int start_node = 0; // 'a'
    int target_node = 5; // 'f'
    
    dijkstra(start_node, target_node, adj);
    aStar(start_node, target_node, adj, h);
    
    return 0;
}