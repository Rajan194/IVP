#include <bits/stdc++.h>
using namespace std;

int n, m, best = INT_MAX;
vector<vector<pair<int,int>>> jobs;
vector<int> ord;

void eval() {
    vector<int> done(n, 0), jt(n, 0), mt(m, 0);
    for (int j : ord) {
        int s = jt[j];
        for (int k = 0; k < done[j]; k++)
            s = max(s, mt[jobs[j][k].second]);
        auto [mach, dur] = jobs[j][done[j]];
        s = max(s, mt[mach]);
        mt[mach] = s + dur;
        jt[j] = s + dur;
        done[j]++;
    }
    int mx = 0;
    for (int t : mt) mx = max(mx, t);
    best = min(best, mx);
}

void perm(int i) {
    if (i == (int)ord.size()) { eval(); return; }
    for (int k = i; k < (int)ord.size(); k++) {
        swap(ord[i], ord[k]);
        perm(i + 1);
        swap(ord[i], ord[k]);
    }
}

int main() {
    cin >> n >> m;
    jobs.resize(n);
    for (int j = 0; j < n; j++)
        for (int k = 0; k < m; k++) {
            int mach, dur;
            cin >> mach >> dur;
            jobs[j].push_back({mach, dur});
        }
    for (int j = 0; j < n; j++)
        for (int k = 0; k < m; k++) ord.push_back(j);
    sort(ord.begin(), ord.end());
    perm(0);
    cout << best << "\n";
    return 0;
}