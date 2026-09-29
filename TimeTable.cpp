#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

// 0=AI, 1=DBMS, 2=OS, 3=CN
const int NUM_COURSES = 4;
vector<string> courses = {"AI", "DBMS", "OS", "CN"};

// AI!=DBMS, AI!=OS, DBMS!=CN, OS!=CN
vector<vector<int>> conflicts = {
    {1, 2},    // AI conflicts with DBMS, OS
    {0, 3},    // DBMS conflicts with AI, CN
    {0, 3},    // OS conflicts with AI, CN
    {1, 2}     // CN conflicts with DBMS, OS
};

void printDomains(const vector<set<int>>& domains) {
    cout << "Course\t| Domain\n";
    for (int i = 0; i < NUM_COURSES; i++) {
        cout << courses[i] << "\t| {";
        auto it = domains[i].begin();
        while (it != domains[i].end()) {
            cout << *it;
            ++it;
            if (it != domains[i].end()) cout << ",";
        }
        cout << "}\n";
    }
    cout << "-----------------------\n";
}

int getMRV(const vector<set<int>>& domains, const vector<int>& assigned) {
    int min_size = 10, best_c = -1;
    for (int i = 0; i < NUM_COURSES; i++) {
        if (assigned[i] != 0) continue; 
        if (domains[i].size() < min_size) {
            min_size = domains[i].size();
            best_c = i;
        }
    }
    return best_c;
}

bool forwardCheckAssign(int c, int val, vector<set<int>>& domains, const vector<int>& assigned) {
    // Remove val from conflicting neighbors' domains
    for (int neighbor : conflicts[c]) {
        if (assigned[neighbor] == 0) {
            domains[neighbor].erase(val);
            if (domains[neighbor].empty()) return false; // Domain wipeout
        }
    }
    return true;
}

bool solveMRV_FC(int step, vector<int>& assigned, vector<set<int>> domains) {
    if (step == NUM_COURSES) return true;

    int c = getMRV(domains, assigned);
    
    // Copy the available values to iterate so we don't invalidate iterators 
    set<int> available_vals = domains[c];
    
    for (int val : available_vals) {
        cout << "1. Variable selected using MRV: " << courses[c] << "\n";
        cout << "2. Value assigned: " << val << "\n";
        
        assigned[c] = val;
        
        // Save state before forward checking
        vector<set<int>> temp_domains = domains;
        temp_domains[c].clear();
        temp_domains[c].insert(val); // Assigned domain has 1 value
        
        bool fc_success = forwardCheckAssign(c, val, temp_domains, assigned);
        
        cout << "3. Updated domains after Forward Checking:\n";
        printDomains(temp_domains);
        
        if (!fc_success) {
            cout << "4. Backtracking required: Yes (Domain wipeout detected)\n\n";
            assigned[c] = 0; 
            continue;
        }
        
        cout << "4. Backtracking required: No\n\n";
        
        if (solveMRV_FC(step + 1, assigned, temp_domains)) return true;
        
        // Backtrack
        assigned[c] = 0;
        cout << "Backtracking from " << courses[c] << "=" << val << "\n\n";
    }
    
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<set<int>> initial_domains = {
        {1, 2, 3},    // AI
        {1, 2, 4},    // DBMS
        {2, 3, 4},    // OS
        {1, 3, 4}     // CN
    };

    cout << "Initial Domains:\n";
    printDomains(initial_domains);
    cout << "\n";

    vector<int> assigned(NUM_COURSES, 0);
    
    if (solveMRV_FC(0, assigned, initial_domains)) {
        cout << "=== Final Valid Timetable ===\n";
        for (int i = 0; i < NUM_COURSES; i++) {
            cout << courses[i] << " -> Slot " << assigned[i] << "\n";
        }
    } else {
        cout << "No valid assignment found.\n";
    }

    return 0;
}
/*
The following constraints apply:
● AI and DBMS cannot be scheduled at the same time.
● AI and OS cannot be scheduled at the same time.
● DBMS and CN cannot be scheduled at the same time.
● OS and CN cannot be scheduled at the same time.
● AI can be scheduled only in slots 1, 2, or 3.
● DBMS can be scheduled only in slots 1, 2, or 4.
● OS can be scheduled only in slots 2, 3, or 4.
● CN can be scheduled only in slots 1, 3, or 4.
*/