#include <bits/stdc++.h>
using namespace std;

int n, m, k;
vector<vector<int>> adj;
vector<int> clr;

bool solve(int v) {
    if (v == n) return true;
    for (int c = 1; c <= k; c++) {
        bool ok = true;
        for (int u : adj[v])
            if (clr[u] == c) { ok = false; break; }
        if (!ok) continue;
        clr[v] = c;
        if (solve(v + 1)) return true;
        clr[v] = 0;
    }
    return false;
}

int main() {
    cin >> n >> m >> k;
    adj.assign(n, {});
    clr.assign(n, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (solve(0))
        for (int i = 0; i < n; i++) cout << clr[i] << " ";
    else
        cout << "no solution";
    cout << "\n";
    return 0;
}