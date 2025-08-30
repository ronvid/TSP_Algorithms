#include <map>
#include <iostream>

/*
 * This file adds utility functions for constructing, copying and deleting graphs
 */

// adds an edge between v and w with cost c
// since the graph structure does not allow parallel edges, the edge with the smaller cost will be used
void add_edge(std::map<int, std::map<int, int>>* W, int v, int w, int c){
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
void copy_graph(std::map<int, std::map<int, bool>>* G, std::map<int, std::map<int, bool>>* T){
    for(const auto& [v, e] : *G){
        for(const auto& [w, og] : e){
            (*T)[v][w] = og;
        }
    }
}

// clears values from a given graph
void delete_graph(std::map<int, std::map<int, bool>>* G){
    G->clear();
}

// prints the given graph
void print_graph(std::map<int, std::map<int, bool>>* G){
    for(const auto& [v, e] : *G){
        std::cout << v << ": ";
        for(const auto& [w, og] : e){
            std::cout << w << "." << og << " ";
        }
        std::cout << std::endl;
    }
}

// prints the given weight graph
void print_graph(std::map<int, std::map<int, int>>* W){
    for(const auto& [v, e] : *W){
        std::cout << v << ": ";
        for(const auto& [w, og] : e){
            std::cout << w << "." << og << " ";
        }
        std::cout << std::endl;
    }
}
