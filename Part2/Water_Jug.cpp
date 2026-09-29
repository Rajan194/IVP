#include <bits/stdc++.h>
using namespace std;

int capA, capB, goal;
map<pair<int,int>, bool> seen;
map<pair<int,int>, pair<int,int>> prevStep;

void printPath(int x, int y) {
    vector<pair<int,int>> path;
    while (make_pair(x, y) != make_pair(-1, -1)) {
        path.push_back({x, y});
        auto p = prevStep[{x, y}];
        x = p.first; y = p.second;
    }
    reverse(path.begin(), path.end());
    for (auto &s : path) cout << s.first << " " << s.second << "\n";
}

void solve() {
    queue<pair<int,int>> q;
    q.push({0, 0});
    seen[{0, 0}] = true;
    prevStep[{0, 0}] = {-1, -1};
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == goal || y == goal) { printPath(x, y); return; }
        vector<pair<int,int>> moves = {
            {capA, y}, {x, capB}, {0, y}, {x, 0},
            {max(0, x - (capB - y)), min(capB, x + y)},
            {min(capA, x + y), max(0, y - (capA - x))}
        };
        for (auto &m : moves) {
            if (!seen[m]) {
                seen[m] = true;
                prevStep[m] = {x, y};
                q.push(m);
            }
        }
    }
    cout << "no solution\n";
}

int main() {
    cin >> capA >> capB >> goal;
    solve();
    return 0;
}