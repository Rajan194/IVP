#include <bits/stdc++.h>
using namespace std;

string a, b, c;
vector<char> ltr;
map<char,int> val;
set<char> lead;

bool check() {
    int x = 0, y = 0, z = 0;
    for (char ch : a) x = x * 10 + val[ch];
    for (char ch : b) y = y * 10 + val[ch];
    for (char ch : c) z = z * 10 + val[ch];
    return x + y == z;
}

bool solve(int i) {
    if (i == (int)ltr.size()) return check();
    for (int d = 0; d <= 9; d++) {
        if (lead.count(ltr[i]) && d == 0) continue;
        bool used = false;
        for (auto &p : val)
            if (p.second == d) { used = true; break; }
        if (used) continue;
        val[ltr[i]] = d;
        if (solve(i + 1)) return true;
        val.erase(ltr[i]);
    }
    return false;
}

int main() {
    cin >> a >> b >> c;
    lead.insert(a[0]);
    lead.insert(b[0]);
    lead.insert(c[0]);
    set<char> s;
    for (char ch : a) s.insert(ch);
    for (char ch : b) s.insert(ch);
    for (char ch : c) s.insert(ch);
    ltr.assign(s.begin(), s.end());
    if (solve(0))
        for (char ch : ltr) cout << ch << "=" << val[ch] << " ";
    else
        cout << "no solution";
    cout << "\n";
    return 0;
}