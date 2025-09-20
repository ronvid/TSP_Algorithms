#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <climits>

void recursive_bruteforce(int v);
void BruteforceHamiltonianCycle(std::unordered_map<int, std::unordered_map<int, int>>* input_weight, std::unordered_map<int, std::unordered_map<int, bool>>* solution, int* cost);

// the weight graph
std::unordered_map<int, std::unordered_map<int, int>> W;

// the recursivley constructed soluion graph
std::unordered_map<int, std::set<int>> R;

// the best weight solution graph
std::unordered_map<int, std::unordered_map<int, bool>>* S;

// set of added vertices
std::set<int> added_vertices;

// the stack of added vertices (for backtracing)
std::vector<int> vertex_stack;

// amount of vertices in original graph
int n;

// total cost of the current solution graph
int current_cost = 0;

// the best weight of a solution graph
int min_found_cost = INT_MAX;


void recursive_bruteforce(int v){
    //if v is the same as the first added vertex and there are n vertices added -> solution
    if(added_vertices.size() == n && vertex_stack.front() == v){
        if(current_cost < min_found_cost){
            min_found_cost = current_cost;
            // current best solution found, copy R to S
            S->clear();
            for(const auto& [v, e] : R){
                for(int w : e){
                    (*S)[v][w] = true;
                }
            }
        }
    }
    // check if v was already added to the solution graph
    if(added_vertices.contains(v)){
        return; // v already added
    }

    added_vertices.insert(v); // add v to added vertices
    vertex_stack.push_back(v); // add v to vertex stack

    // for every edge adjacent to v, run the function recursivley, remove edge after function
    for(const auto& [w, c] : W[v]){
        if(R[v].contains(w)) continue; // check if edge already exists

        R[v].insert(w);
        R[w].insert(v);
        current_cost += W[v][w]; // add cost of edge to total cost

        recursive_bruteforce(w);

        current_cost -= W[v][w];
        R[v].erase(w);
        R[w].erase(v);
    }

    // remove v from added vertices
    added_vertices.erase(v);
    vertex_stack.pop_back();
}

void BruteforceHamiltonianCycle(std::unordered_map<int, std::unordered_map<int, int>>* input_weight,
                                std::unordered_map<int, std::unordered_map<int, bool>>* solution, int* cost){

    // assign weight and solution graph
    W = *input_weight;
    S = solution;
    // assign number of vertices
    n = W.size();

    // initialize solution graph
    for(const auto& [v, e] : W){
        R[v];
    }

    // start function with first vertex of W
    int v = (*(W.begin())).first;
    recursive_bruteforce(v);
    // copy found bets weight
    *cost = min_found_cost;
}

#endif
