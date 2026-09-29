#include <iostream>
#include <vector>
#include <algorithm>

struct GameState {
    int score; // Valid only for leaves
    std::vector<GameState> children;
    
    bool isTerminal() const {
        return children.empty();
    }
    int getScore() const {
        return score;
    }
    std::vector<GameState> getPossibleMoves() const {
        return children;
    }
};

int Minimax(const GameState& state, int depth, bool isMaximizingPlayer) {
    if (depth == 0 || state.isTerminal()) return state.getScore();

    if (isMaximizingPlayer) {
        int bestVal = -1e9;
        for (const GameState& child : state.getPossibleMoves()) {
            int value = Minimax(child, depth - 1, false);
            bestVal = std::max(bestVal, value);
        }
        return bestVal;
    } else {
        int bestVal = 1e9;
        for (const GameState& child : state.getPossibleMoves()) {
            int value = Minimax(child, depth - 1, true);
            bestVal = std::min(bestVal, value);
        }
        return bestVal;
    }
}

int AlphaBetaPruning(const GameState& state, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || state.isTerminal()) return state.getScore();

    if (isMaximizingPlayer) {
        int maxEval = -1e9;
        for (const GameState& child : state.getPossibleMoves()) {
            int eval = AlphaBetaPruning(child, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 1e9;
        for (const GameState& child : state.getPossibleMoves()) {
            int eval = AlphaBetaPruning(child, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int main() {
    // Constructing a standard decision tree
    // Root -> (Left -> (3, 5)), (Right -> (2, 9))
    GameState leaf1{3, {}};
    GameState leaf2{5, {}};
    GameState leaf3{2, {}};
    GameState leaf4{9, {}};
    
    GameState node1{0, {leaf1, leaf2}};
    GameState node2{0, {leaf3, leaf4}};
    
    GameState root{0, {node1, node2}};
    
    int minimax_result = Minimax(root, 2, true);
    int ab_result = AlphaBetaPruning(root, 2, -1e9, 1e9, true);
    
    std::cout << "Minimax Optimal Score: " << minimax_result << "\n";
    std::cout << "Alpha-Beta Optimal Score: " << ab_result << "\n";
    
    return 0;
}