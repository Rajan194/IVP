#include <bits/stdc++.h>
using namespace std;

int n, m, k;
vector<vector<int>> adj;
vector<int> slot;

bool solve(int v) {
    if (v == n) return true;
    for (int s = 1; s <= k; s++) {
        bool ok = true;
        for (int u : adj[v])
            if (slot[u] == s) { ok = false; break; }
        if (!ok) continue;
        slot[v] = s;
        if (solve(v + 1)) return true;
        slot[v] = 0;
    }
    return false;
}

int main() {
    cin >> n >> m >> k;
    adj.assign(n, {});
    slot.assign(n, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (solve(0))
        for (int i = 0; i < n; i++) cout << slot[i] << " ";
    else
        cout << "no solution";
    cout << "\n";
    return 0;
}