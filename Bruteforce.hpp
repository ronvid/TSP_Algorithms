#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <climits>

class Brutefoce{

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

};

#endif
