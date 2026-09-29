#include <iostream>
#include <vector>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

const int N = 9;

// Constraint check
bool isSafe(const vector<vector<int>>& grid, int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return false;
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i + startRow][j + startCol] == num) return false;
        }
    }
    return true;
}

// MRV Heuristic: Find the empty cell with the smallest domain
bool findMRVCell(const vector<vector<int>>& grid, int& row, int& col) {
    int min_domain_size = 10; // Max possible is 9
    bool found_empty = false;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                found_empty = true;
                int valid_choices = 0;
                
                // Calculate domain size for this specific cell
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(grid, r, c, num)) {
                        valid_choices++;
                    }
                }

                // Update MRV target
                if (valid_choices < min_domain_size) {
                    min_domain_size = valid_choices;
                    row = r;
                    col = c;
                }
            }
        }
    }
    return found_empty;
}

bool solveSudoku(vector<vector<int>>& grid) {
    int row, col;
    
    // If no empty cells are left, puzzle is solved
    if (!findMRVCell(grid, row, col)) {
        return true; 
    }

    // Try assigning valid numbers to the cell selected by MRV
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num; // Forward Assignment

            if (solveSudoku(grid)) {
                return true;
            }

            grid[row][col] = 0; // Backtracking
        }
    }
    return false; // Triggers backtrack to previous cell
}

void printGrid(const vector<vector<int>>& grid) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) cout << grid[r][c] << " ";
        cout << "\n";
    }
}

int main() {
    fast_io;
    vector<vector<int>> grid = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    if (solveSudoku(grid)) printGrid(grid);
    else cout << "No solution exists.\n";

    return 0;
}