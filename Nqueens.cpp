#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

const int N = 8;
vector<int> board(N, -1); // board[i] = column of queen in row i

// Forward Checking: Determine if a specific (row, col) is safe
bool isSafe(int row, int col) {
    for (int i = 0; i < row; i++) {
        if (board[i] != -1) {
            // Check column and diagonals
            if (board[i] == col || abs(board[i] - col) == abs(i - row)) return false;
        }
    }
    return true;
}

// LCV Heuristic: Count how many valid spots remain in future rows if we pick (r, c)
int countRemainingChoices(int row, int col) {
    int remaining = 0;
    board[row] = col; // Temporarily assign to test future impact
    
    for (int r = row + 1; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (isSafe(r, c)) remaining++;
        }
    }
    
    board[row] = -1; // Undo temporary assignment
    return remaining;
}

bool solveNQueensLCV(int row) {
    if (row == N) return true; // All queens placed

    // 1. Gather all valid column assignments for this row (Domain filtering)
    vector<pair<int, int>> valid_moves; // {remaining_choices (LCV score), column}
    
    for (int col = 0; col < N; col++) {
        if (isSafe(row, col)) {
            int score = countRemainingChoices(row, col);
            valid_moves.push_back({score, col});
        }
    }

    // 2. MRV/Domain Wipeout Check: If no valid columns exist for this row, fail early
    if (valid_moves.empty()) return false; 

    // 3. LCV Ordering: Sort descending by score (prioritize moves leaving max options)
    sort(valid_moves.rbegin(), valid_moves.rend());

    // 4. Backtracking Search using LCV-ordered values
    for (auto move : valid_moves) {
        int col = move.second;
        
        board[row] = col; // Assign
        
        if (solveNQueensLCV(row + 1)) {
            return true;
        }
        
        board[row] = -1;  // Backtrack
    }
    
    return false;
}

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i] == j) cout << "Q ";
            else cout << ". ";
        }
        cout << "\n";
    }
}

int main() {
    fast_io;
    if (solveNQueensLCV(0)) {
        cout << "N-Queens Solution (Backtracking + MRV + LCV):\n";
        printBoard();
    } else {
        cout << "No solution exists.\n";
    }
    return 0;
}