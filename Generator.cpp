#include "Bruteforce.hpp"
#include "GraphUtility.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

enum generation{
    RANDOM_CYCLE = 0,
    RANDOM_REGULAR = 1,
    HIGH_HAMILTONIAN = 2
};

void random_cycle(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);
void random_regular(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost);
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
    else if(type == RANDOM_REGULAR){
        random_regular(W, n, max_cost);
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

void random_regular(std::unordered_map<int, std::unordered_map<int, int>>* W, int n, int max_cost){
    // create random regular graphs

    // insert vertices to the graph
    for(int i = 0; i < n; i++){
        (*W)[i];
    }

    // array with the value of the points
    int L[n*3];
    // array where the i'th value corresponds to the place of i in the array L
    int I[n*3];
    // set values for L and I
    for(int i = 0; i < n*3; i++){
        L[i] = i;
        I[i] = i;
    }
    // number of unused points
    int U = n*3;

phase_one:
    // first phase, while U >= 2d^2 == 18
    while(U >= 18){
        // get two random points that are not jet used
        int i = get_bounded_random(U);
        int j = get_bounded_random(U);
        std::cout << "selected: i=" << i << " j=" << j << std::endl;
        // check that the values of i and j don't belong to the same group
        if(int(L[i]/3) == int(L[j]/3)){
            std::cout << "i and j in same group!" << std::endl;
            continue;
        }
        // check that i or j are not yet matched
        if(I[i] >= U || I[j] >= U){ std::cout << "i or j already matched" << std::endl; continue; }
        // check that two other points of the groups are not jet matched with each other
        for(int x = 0; x < 3; x++){
            int g = int(L[i]/3)*3; // the first value of the group that L[i] belongs to
            if(I[g+x] >= U && I[g+x]%2 == 0){ // check if other points of the group are matched, and if the matched value is at an even place
                if(int(L[I[g+x]+1]/3) == int(L[j]/3)){std::cout << "-- already connected!" << std::endl; goto phase_one; }// check if other value of the pair is part of j's group
            }
            else if(I[g+x] >= U && I[g+x]%2 == 1){ // check if other points of the group are matched, and if the matched value is at an even place
                if(int(L[I[g+x]-1]/3) == int(L[j]/3)){std::cout << "-- already connected!" << std::endl; goto phase_one; }// check if other value of the pair is part of j's group
            }
        }
        // swap the values of i and j, with the last to elements in U
        int tmp = L[U-1]; L[U-1] = L[i]; L[i] = tmp; // swap L values
        I[i] = U-1; I[L[i]] = i; // swap I values
        tmp = L[U-2]; L[U-2] = L[j]; L[j] = tmp; // swap L values
        I[j] = U-2; I[L[j]] = j; // swap I values
        U -= 2;

        std::cout << "new values: U=" << U << "\nL: ";
        for(int x = 0; x < n*3; x++){std::cout << L[x] << " ";} std::cout << "\nI: ";
        for(int x = 0; x < n*3; x++){std::cout << I[x] << " ";} std::cout << std::endl;
    }

    std::cout << "phase two" << std::endl;
phase_two:
    // second phase, while U >= 2d == 6
    while(U >= 6){
        //randomly pic groups instead of points and check if they can be connected
        int v = get_bounded_random(n)*3; // set to the first element of the group
        int u = get_bounded_random(n)*3;
        std::cout << "selected groups: v=" << v << " u=" << u << std::endl;
        if(v == u){
            std::cout << "v and u are equal!" << std::endl;
        }
        // check if at least one point in each group is unconnected
        if(I[v+0] >= U && I[v+1] >= U && I[v+2] >= U){ std::cout << "v fully connected" << std::endl; continue; }
        if(I[u+0] >= U && I[u+1] >= U && I[u+2] >= U){ std::cout << "u fully connected" << std::endl; continue; }

        // check if v and u are already connected
        for(int x = 0; x < 3; x++){
            for(int y = 0; y < 3; y++){
                if(v+x >= U && I[v+x]%2 == 0){ // check if point v+x is already selected and if its value is at an even place
                    if(L[I[v+x]+1] == u+y){ std::cout << "-- already connected!" << std::endl; goto phase_two; }
                }
                else if(v+x >= U && I[x+v]%2 == 1){ // check if point v+x is already selected and if its value is at an even place
                    if(L[I[v+x]-1] == u+y){ std::cout << "-- already connected!" << std::endl; goto phase_two; }
                }
            }
        }
        // pick a random point in each group that is not yet connected
        int i, j;
        do{
            i = v + get_bounded_random(3);
            j = u + get_bounded_random(3);
        }while(I[i] >= U || I[u] >= U);

        std::cout << "selected points: i=" << i << " j=" << j << " with U=" << U << std::endl;
        // swap the values of i and j, with the last to elements in U
        int tmp = L[U-1]; L[U-1] = L[i]; L[i] = tmp; // swap L values
        I[i] = U-1; I[L[i]] = i; // swap I values
        tmp = L[U-2]; L[U-2] = L[j]; L[j] = tmp; // swap L values
        I[j] = U-2; I[L[j]] = j; // swap I values
        U -= 2;

        std::cout << "new values: U=" << U << "\nL: ";
        for(int x = 0; x < n*3; x++){std::cout << L[x] << " ";} std::cout << "\nI: ";
        for(int x = 0; x < n*3; x++){std::cout << I[x] << " ";} std::cout << std::endl;
    }
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
