#include <bits/stdc++.h>
using namespace std;

int g[9][9];
bool row[9][10], col[9][10], box[9][10];

int bid(int r, int c) {
    return (r / 3) * 3 + c / 3;
}

bool solve(int p) {
    if (p == 81) return true;
    int r = p / 9, c = p % 9;
    if (g[r][c]) return solve(p + 1);
    for (int v = 1; v <= 9; v++) {
        if (row[r][v] || col[c][v] || box[bid(r, c)][v]) continue;
        g[r][c] = v;
        row[r][v] = col[c][v] = box[bid(r, c)][v] = true;
        if (solve(p + 1)) return true;
        g[r][c] = 0;
        row[r][v] = col[c][v] = box[bid(r, c)][v] = false;
    }
    return false;
}

int main() {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++) {
            cin >> g[r][c];
            if (g[r][c]) {
                int v = g[r][c];
                row[r][v] = col[c][v] = box[bid(r, c)][v] = true;
            }
        }
    solve(0);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) cout << g[r][c] << " ";
        cout << "\n";
    }
    return 0;
}