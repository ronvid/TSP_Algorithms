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
    static void remove(int v, int w);
    static void now_degree_two(int v);
    static bool safely_remove(int u, int w);
    static bool remove_third_leg(int v);
    static bool force(int v, int w);
    static bool force_into_triangle(int v, int w);
    static bool contract(int v);
    static bool handle_degree_two();

    static bool ShortestHamiltonianCycle(std::unordered_map<int, std::unordered_map<int, int>>* input_weights, std::unordered_map<int, std::unordered_map<int, bool>>* forced_edges, int* cost);

    //void print_graph(std::map<int, std::map<int, bool>>* G);

    // a copy of the given graph to run the algorithm on
    static std::unordered_map<int, std::unordered_map<int, bool>> G;

    // a copy of the given weights to run the algorithm on
    static std::unordered_map<int, std::unordered_map<int, int>> W;

    // keeps track of the weight of the current forced edges
    static int current_weight;
    // the weight of the current best weight graph
    static int min_found_weight;

    // a subgraph of forced edges in original G
    static std::unordered_map<int, std::unordered_map<int, bool>> forced_in_input;

    // a subgraph of forced edges in current G
    static std::unordered_map<int, std::unordered_map<int, bool>> forced_in_current;

    // set of vertices that are degree_two
    static std::vector<int> degree_two;

    // set of all vertices that are adjacent to a forced edge
    static std::unordered_map<int, bool> forced_vertices;

    // the action "stack"
    static std::vector<std::function<bool()>> actions;

    static int get_unforced_neighbour(int v);

};

#endif
