#include <iostream>
#include <vector>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

const int N = 4;

// Forward checking state
bool row_used[N][N + 1] = {false};
bool col_used[N][N + 1] = {false};

// MRV: Find the cell with the smallest domain size
bool findMRVCell(const vector<vector<int>>& grid, int& best_r, int& best_c) {
    int min_options = N + 1;
    bool found = false;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) { // Unassigned
                int options = 0;
                for (int val = 1; val <= N; val++) {
                    if (!row_used[r][val] && !col_used[c][val]) {
                        options++;
                    }
                }
                
                // Track the cell with the absolute minimum remaining values
                if (options < min_options) {
                    min_options = options;
                    best_r = r;
                    best_c = c;
                    found = true;
                }
            }
        }
    }
    return found;
}

bool solveLatinSquare(vector<vector<int>>& grid) {
    int r, c;
    
    // If no empty cells are left, we have a complete valid assignment
    if (!findMRVCell(grid, r, c)) {
        return true; 
    }

    // Try assigning values to the MRV-selected cell
    for (int val = 1; val <= N; val++) {
        if (!row_used[r][val] && !col_used[c][val]) {
            
            // 1. Assign and Apply Forward Checking Update
            grid[r][c] = val;
            row_used[r][val] = true;
            col_used[c][val] = true;

            // 2. Recurse
            if (solveLatinSquare(grid)) return true;

            // 3. Backtrack and Restore Domain
            grid[r][c] = 0;
            row_used[r][val] = false;
            col_used[c][val] = false;
        }
    }
    return false;
}

int main() {
    fast_io;
    vector<vector<int>> grid(N, vector<int>(N, 0));
    
    // Pre-fill a few constraints
    grid[0][0] = 1; row_used[0][1] = true; col_used[0][1] = true;
    grid[1][1] = 2; row_used[1][2] = true; col_used[1][2] = true;

    if (solveLatinSquare(grid)) {
        cout << "Latin Square Solution:\n";
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) cout << grid[r][c] << " ";
            cout << "\n";
        }
    } else {
        cout << "No solution exists.\n";
    }
    return 0;
}