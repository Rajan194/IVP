#include <vector>
#include <algorithm>
#include <queue>

using Variables = std::vector<int>;
using Domains = std::vector<std::vector<int>>;
using ConstraintGraph = std::vector<std::vector<int>>;

// Dummy constraint evaluator
bool satisfiesConstraint(int var1, int val1, int var2, int val2) {
    return val1 != val2; // Example: Map Coloring constraint
}

// --- HEURISTICS ---

// MRV combined with Degree Heuristic for tie-breaking
int selectUnassignedVariable(const Variables& assignment, 
                             const Domains& domains, 
                             const ConstraintGraph& cg) {
    int best_var = -1;
    int min_domain_size = 1e9;
    int max_degree = -1;
    
    for (size_t i = 0; i < assignment.size(); ++i) {
        if (assignment[i] == -1) { // Variable is unassigned
            int domain_size = domains[i].size();
            
            // Calculate degree (number of unassigned neighbors)
            int degree = 0;
            for (int neighbor : cg[i]) {
                if (assignment[neighbor] == -1) degree++;
            }
            
            // MRV check
            if (domain_size < min_domain_size) {
                min_domain_size = domain_size;
                max_degree = degree;
                best_var = i;
            } 
            // Degree Heuristic tie-breaker
            else if (domain_size == min_domain_size && degree > max_degree) {
                max_degree = degree;
                best_var = i;
            }
        }
    }
    return best_var;
}

// Least Constraining Value (LCV)
std::vector<int> orderDomainValues(int var, const Variables& assignment, 
                                   const Domains& domains, const ConstraintGraph& cg) {
    std::vector<int> values = domains[var];
    
    // Pair of {eliminated_choices_count, value}
    std::vector<std::pair<int, int>> value_impacts;
    
    for (int val : values) {
        int eliminated = 0;
        // Check impact on all unassigned neighbors
        for (int neighbor : cg[var]) {
            if (assignment[neighbor] == -1) {
                for (int neighbor_val : domains[neighbor]) {
                    if (!satisfiesConstraint(var, val, neighbor, neighbor_val)) {
                        eliminated++;
                    }
                }
            }
        }
        value_impacts.push_back({eliminated, val});
    }
    
    // Sort values by least impact (ascending order of eliminated choices)
    std::sort(value_impacts.begin(), value_impacts.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.first < b.first;
              });
              
    std::vector<int> sorted_values;
    for (const auto& pair : value_impacts) {
        sorted_values.push_back(pair.second);
    }
    return sorted_values;
}

// --- INFERENCE ---

// Forward Checking (FC)
// Called immediately after assigning `val` to `var`.
bool forwardChecking(int var, int val, Domains& domains, const Variables& assignment, const ConstraintGraph& cg) {
    for (int neighbor : cg[var]) {
        if (assignment[neighbor] == -1) { // Only check unassigned neighbors
            // Remove values from neighbor's domain that are inconsistent with var = val
            domains[neighbor].erase(
                std::remove_if(domains[neighbor].begin(), domains[neighbor].end(),
                    [&](int neighbor_val) {
                        return !satisfiesConstraint(var, val, neighbor, neighbor_val);
                    }),
                domains[neighbor].end()
            );
            
            // If any neighbor's domain is wiped out, this path fails
            if (domains[neighbor].empty()) {
                return false;
            }
        }
    }
    return true;
}

// Helper function for AC-3
bool Revise(int xi, int xj, Domains& domains) {
    bool revised = false;
    auto& domain_xi = domains[xi];
    const auto& domain_xj = domains[xj];
    
    for (auto it = domain_xi.begin(); it != domain_xi.end(); ) {
        int x_val = *it;
        bool has_support = false;
        
        for (int y_val : domain_xj) {
            if (satisfiesConstraint(xi, x_val, xj, y_val)) {
                has_support = true;
                break;
            }
        }
        
        if (!has_support) {
            it = domain_xi.erase(it); // Remove unsupported value
            revised = true;
        } else {
            ++it;
        }
    }
    return revised;
}

// AC-3 (Arc Consistency)
bool AC3(Domains& domains, const ConstraintGraph& cg) {
    std::queue<std::pair<int, int>> q;
    
    // Initialize queue with all directed arcs in the constraint graph
    for (size_t i = 0; i < cg.size(); ++i) {
        for (int neighbor : cg[i]) {
            q.push({i, neighbor});
        }
    }
    
    while (!q.empty()) {
        int xi = q.front().first;
        int xj = q.front().second;
        q.pop();
        
        if (Revise(xi, xj, domains)) {
            if (domains[xi].empty()) {
                return false; // Domain wipeout, no solution
            }
            
            // If domain of xi is reduced, re-evaluate all neighbors of xi (except xj)
            for (int neighbor : cg[xi]) {
                if (neighbor != xj) {
                    q.push({neighbor, xi});
                }
            }
        }
    }
    return true;
}