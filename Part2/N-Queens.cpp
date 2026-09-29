#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> col, d1, d2, pos;

void solve(int r) {
    if (r == n) {
        for (int i = 0; i < n; i++) cout << pos[i] << " ";
        cout << "\n";
        return;
    }
    for (int c = 0; c < n; c++) {
        if (col[c] || d1[r + c] || d2[r - c + n - 1]) continue;
        col[c] = d1[r + c] = d2[r - c + n - 1] = 1;
        pos[r] = c;
        solve(r + 1);
        col[c] = d1[r + c] = d2[r - c + n - 1] = 0;
    }
}

int main() {
    cin >> n;
    col.assign(n, 0);
    d1.assign(2 * n, 0);
    d2.assign(2 * n, 0);
    pos.assign(n, 0);
    solve(0);
    return 0;
}