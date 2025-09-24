#include "Bruteforce.hpp"
#include "GraphUtility.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

int get_bounded_random(int max){
    return (rand() % max);
}

void write_seed_to_file(int seed, int size){
    std::ofstream seed_file;
    seed_file.open("last_seed.txt");
    seed_file << seed << "/" << size << "\n";
    seed_file.close();
}

std::unordered_map<int, std::unordered_map<int, int>>* generate_random_graph(int n, int seed){
    // takes two random edges and connectes them
    // does not create parallel edges and self loops

    std::srand(seed);

    int max_cost = 10;

    std::cout << "generate graph with: " << n << " vertices!" << std::endl;

    std::unordered_map<int, std::unordered_map<int, int>>* W = new std::unordered_map<int, std::unordered_map<int, int>>;

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


    return W;
}
/*
int main(int argc, char *argv[]){

    if(argc != 2){
        std::cout << "incorrect numbber of arguments! Expected 2, recieved: " << argc << std::endl;
        return -1;
    }
    int size = atoi(argv[1]);

    int seed = std::time({}); //1756732275
    write_seed_to_file(seed, size);

    std::unordered_map<int, std::unordered_map<int, int>>* W = generate_random_graph(size, seed);

    print_graph(W);

    // check if every vertex has has 2 or 3 edges
    for(const auto[v, e] : (*W)){
        if((*W)[v].size() < 2){
            std::cout << "Less than 2 vertices at: " << v << " " << (*W)[v].size() << std::endl;
        }
        if((*W)[v].size() > 3){
            std::cout << "More than 3 vertices at: " << v << " " << (*W)[v].size() << std::endl;
        }
    }

    std::unordered_map<int, std::unordered_map<int, bool>>* f_edges = new std::unordered_map<int, std::unordered_map<int, bool>>;
    int cost;
    //ShortestHamiltonianCycle(W, f_edges, &cost);
    BruteforceHamiltonianCycle(W, f_edges, &cost);

    print_graph(f_edges);
    std::cout << "Final cost: " << cost << std::endl;

    print_graph(f_edges);


    delete W;
    return 0;
}*/
