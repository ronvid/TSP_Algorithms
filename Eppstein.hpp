#ifndef EPPSTEIN_HPP
#define EPPSTEIN_HPP

#include <map>
#include <set>
#include <vector>
#include <functional>
#include <climits>
#include <unordered_map>

#include "GraphUtility.hpp"

class Eppstein{

    // functions
    void remove(int v, int w);
    void now_degree_two(int v);
    bool safely_remove(int u, int w);
    bool remove_third_leg(int v);
    bool force(int v, int w);
    bool force_into_triangle(int v, int w);
    bool contract(int v);
    bool handle_degree_two();

    //void print_graph(std::map<int, std::map<int, bool>>* G);

    // a copy of the given graph to run the algorithm on
    std::unordered_map<int, std::unordered_map<int, bool>> G;

    // a copy of the given weights to run the algorithm on
    std::unordered_map<int, std::unordered_map<int, int>> W;

    // keeps track of the weight of the current forced edges
    int current_weight = 0;
    // the weight of the current best weight graph
    int min_found_weight = INT_MAX;

    // a subgraph of forced edges in original G
    std::unordered_map<int, std::unordered_map<int, bool>> forced_in_input;

    // a subgraph of forced edges in current G
    std::unordered_map<int, std::unordered_map<int, bool>> forced_in_current;

    // set of vertices that are degree_two
    std::vector<int> degree_two;

    // set of all vertices that are adjacent to a forced edge
    std::unordered_map<int, bool> forced_vertices;

    // the action "stack"
    std::vector<std::function<bool()>> actions = {};

    int get_unforced_neighbour(int v);

};

#endif
