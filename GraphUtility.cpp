#include "GraphUtility.hpp"

/*
 * This file adds utility functions for constructing, copying and deleting graphs
 */

// adds an edge between v and w with cost c
// since the graph structure does not allow parallel edges, the edge with the smaller cost will be used
void add_edge(std::unordered_map<int, std::unordered_map<int, int>>* W, int v, int w, int c){
    // check if an edge was already between v-w
    if((*W)[v].contains(w)){
        // only overwrite cost if new cost would be lower
        if(c < (*W)[v][w]){
            (*W)[v][w] = (*W)[w][v] = c;
        }
    }
    else{
        // add edge and cost
        (*W)[v][w] = (*W)[w][v] = c;
    }
}

// copys edges from G to T
void copy_graph(std::unordered_map<int, std::unordered_map<int, bool>>* G, std::unordered_map<int, std::unordered_map<int, bool>>* T){
    for(const auto& [v, e] : *G){
        for(const auto& [w, og] : e){
            (*T)[v][w] = og;
        }
    }
}

// clears values from a given graph
void delete_graph(std::unordered_map<int, std::unordered_map<int, bool>>* s){
    s->clear();
}

// prints the given graph
void print_graph(std::unordered_map<int, std::unordered_map<int, bool>>* G){
    for(const auto& [v, e] : *G){
        std::cout << v << ": ";
        for(const auto& [w, og] : e){
            std::cout << w << "." << og << " ";
        }
        std::cout << std::endl;
    }
}

// prints the given weight graph
void print_graph(std::unordered_map<int, std::unordered_map<int, int>>* W){
    for(const auto& [v, e] : *W){
        std::cout << v << ": ";
        for(const auto& [w, og] : e){
            std::cout << w << "." << og << " ";
        }
        std::cout << std::endl;
    }
}

void to_dot(std::unordered_map<int, std::unordered_map<int, int>>* W, std::unordered_map<int, std::unordered_map<int, bool>>* forced, std::string file_name){
    // create a graph that saves which edges have already been written to the file (to avoid writing edges twice), probable not optimal, but efficiency does not matter here
    std::unordered_map<int, std::unordered_map<int, int>> G;

    std::ofstream dot_file;
    dot_file.open("vis/" + file_name + ".gv");
    dot_file << "graph g {\n";
    for(const auto& [v, e] : *W){
        for(const auto& [w, c] : e){

            // check if edge was not already added
            if(G[v].contains(w)) continue;
            // add edge to G
            add_edge(&G, v, w, c);

            // add edge to file
            dot_file << v << " -- " << w << "[label=" << c << "]";
            if((*forced)[v].contains(w)){
                dot_file << " [style=bold]";
            }
            dot_file << "\n";
        }
    }
    dot_file << "} \n";
    dot_file.close();
}
