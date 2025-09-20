#ifndef GRAPH_UTILITY_HPP
#define GRAPH_UTILITY_HPP

#include <map>
#include <unordered_map>
#include <iostream>

// adds an edge between v and w with cost c
// since the graph structure does not allow parallel edges, the edge with the smaller cost will be used
void add_edge(std::unordered_map<int, std::unordered_map<int, int>>* W, int v, int w, int c);

// copys edges from G to T
void copy_graph(std::unordered_map<int, std::unordered_map<int, bool>>* G, std::unordered_map<int, std::unordered_map<int, bool>>* T);

// clears values from a given graph
void delete_graph(std::unordered_map<int, std::unordered_map<int, bool>>* s);

// prints the given graph
void print_graph(std::unordered_map<int, std::unordered_map<int, bool>>* G);

// prints the given weight graph
void print_graph(std::unordered_map<int, std::unordered_map<int, int>>* W);

#endif
