#include "Bruteforce.hpp"
#include "GraphUtility.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

enum generation{
    RANDOM_CYCLE = 0,
    RANDOM = 1,
    CAGE = 2,
    HIGH_HAMILTONIAN = 3
};

void random_cycle(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);
void random(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);
void cage(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);
void high_hamiltonian(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);

int get_bounded_random(int max){
    return (rand() % max);
}

void write_seed_to_file(int seed, int size){
    std::ofstream seed_file;
    seed_file.open("last_seed.txt");
    seed_file << seed << "/" << size << "\n";
    seed_file.close();
}

std::unordered_map<int, std::unordered_map<int, int>>* generate_random_graph(int n, int seed, int type){
    // takes two random edges and connectes them
    // does not create parallel edges and self loops

    std::srand(seed);

    int max_cost = 10;

    std::cout << "generate graph with: " << n << " vertices!" << std::endl;

    std::unordered_map<int, std::unordered_map<int, int>>* W = new std::unordered_map<int, std::unordered_map<int, int>>;

    if(type == RANDOM_CYCLE){
        random_cycle(W, n, max_cost);
    }
    else if(type == RANDOM){
        random(W, n, max_cost);
    }
    else if(type == CAGE){
        cage(W, n, max_cost);
    }
    else if(type == HIGH_HAMILTONIAN){
        high_hamiltonian(W, n, max_cost);
    }

    return W;
}

void random_cycle(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost){
    // create a cycle in the graph containing all vertices, than add edges between the vertices that only have two edges randomly

    // insert vertices to the graph
    for(int i = 0; i < n; i++){
        (*W)[i];
    }

    // create a boring cycle and assign random cost
    for(int i = 0; i < n; i++){
        int cost = get_bounded_random(max_cost);
        if(i < n-1){
            add_edge(W, i, i+1, cost);
        }
        else{
            add_edge(W, i, 0, cost);
        }
    }
    // add edges between vertices that don't have three edges yet
    for(int i = 0; i < n; i++){
        if((*W)[i].size() == 3) continue;
        // search for a random vertex to connect i to
        int j = get_bounded_random(n);
        // increment j until a suitable vertex has been found
        for(int o = 0; o < n; o++){
            if(i != (j+o)%n && (*W)[(j+o)%n].size() != 3){
                add_edge(W, i, (j+o)%n, get_bounded_random(max_cost));
                break;
            }
        }
    }
}
void random(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost){
    // create vertices and connect them randomly

    // insert vertices to the graph
    for(int i = 0; i < n; i++){
        (*W)[i];
    }

    // add edges between vertices that don't have three edges yet
    for(int i = 0; i < n; i++){
        while((*W)[i].size() < 3){
            // search for a random vertex to connect i to
            int j = get_bounded_random(n);
            // increment j until a suitable vertex has been found
            for(int o = 0; o < n; o++){
                if(i != (j+o)%n && (*W)[(j+o)%n].size() != 3 && !(*W)[i].contains((j+o)%n)){
                    add_edge(W, i, (j+o)%n, get_bounded_random(max_cost));
                    break;
                }
                if(o == n-1){
                    // if no other suitable vertex was found, go to next vertex
                    i++;
                    goto check_vertex;
                }
            }
        }
        check_vertex:;
    }
}
void cage(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost){

}
void high_hamiltonian(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost){
    // create gadgets of size six an connect them with with a single edge

    // graph size needs to be a multiple of 6
    if(n%6 != 0) return;

    // insert vertices to the graph
    for(int i = 0; i < n; i++){
        (*W)[i];
    }

    for(int i = 0; i < n; i+=6){

        add_edge(W, i  , i+1, get_bounded_random(max_cost));
        add_edge(W, i  , i+2, get_bounded_random(max_cost));
        add_edge(W, i+1, i+3, get_bounded_random(max_cost));
        add_edge(W, i+1, i+4, get_bounded_random(max_cost));
        add_edge(W, i+2, i+3, get_bounded_random(max_cost));
        add_edge(W, i+2, i+4, get_bounded_random(max_cost));
        add_edge(W, i+3, i+5, get_bounded_random(max_cost));
        add_edge(W, i+4, i+5, get_bounded_random(max_cost));

        // connect last vertex in the gadget with the next gadget, or if it is the last vertex in W, connect it to the first vertex
        if(i+5 != n-1){
            add_edge(W, i+5, i+6, get_bounded_random(max_cost));
        }
        else{
            add_edge(W, i+5, 0, get_bounded_random(max_cost));
        }
    }

}
