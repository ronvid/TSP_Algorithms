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

    // needs to be a multiple of two (TODO could also not be a multiple of two, then one vertex would be of degree 2)
    if(n%2 != 0) return;

    // create random regular graphs

begin:
    // insert vertices to the graph
    for(int i = 0; i < n; i++){
        (*W)[i];
    }

    int tmp;
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

    // first phase, while U >= 2d^2 == 18
    while(U >= 18){
        // get two random points that are not jet used
        int i = get_bounded_random(U);
        int j = get_bounded_random(U);
        // check that the values of i and j don't belong to the same group
        if(int(L[i]/3) == int(L[j]/3)) continue;

        // check that i or j are not yet matched
        if(I[i] >= U || I[j] >= U) continue;

        // check that two other points of the groups are not jet matched with each other
        if((*W)[I[i]].contains(I[j])) continue;

        // swap the values of i and j, with the last to elements in U
        tmp = L[U-1]; L[U-1] = L[i]; L[i] = tmp; // swap L values
        I[i] = U-1; I[L[i]] = i; // swap I values
        tmp = L[U-2]; L[U-2] = L[j]; L[j] = tmp; // swap L values
        I[j] = U-2; I[L[j]] = j; // swap I values
        // add the edge to W with random cost
        add_edge(W, int(L[i]/3), int(L[j]/3), get_bounded_random(max_cost));
        U -= 2;

    }
    std::cout << "----- phase one -----" << std::endl;
    print_graph(W);

    // second phase, while U >= 2d == 6
    while(U >= 6){
        //randomly pic groups instead of points and check if they can be connected
        int v = get_bounded_random(n)*3; // set to the first element of the group
        int u = get_bounded_random(n)*3;
        if(v == u) continue;

        // check if at least one point in each group is unconnected
        if(I[v+0] >= U && I[v+1] >= U && I[v+2] >= U) continue;
        if(I[u+0] >= U && I[u+1] >= U && I[u+2] >= U) continue;

        // check if v and u are already connected
        if((*W)[v/3].contains(u/3)) continue;

        // pick a random point in each group that is not yet connected
        int i, j;
        do{
            i = v + get_bounded_random(3);
        }while(I[i] >= U);
        do{
            j = u + get_bounded_random(3);
        }while(I[j] >= U);

        // swap the values of i and j, with the last to elements in U
        tmp = L[U-1]; L[U-1] = L[I[i]]; L[I[i]] = tmp; // swap L values
        I[tmp] = I[i]; I[i] = U-1; // swap I values
        tmp = L[U-2]; L[U-2] = L[I[j]]; L[I[j]] = tmp; // swap L values
        I[tmp] = I[j]; I[j] = U-2; // swap I values
        // add the edge to W with random cost
        add_edge(W, int(i/3), int(j/3), get_bounded_random(max_cost));
        U -= 2;
    }
    std::cout << "----- phase two -----" << std::endl;
    print_graph(W);

    // phase three (TODO TODO this is very messy)
    // find all possible pairs of vertices (d!=3) and add them to a vector
    std::vector<std::pair<int, int>> pairs;

    for(const auto& [v,e] : *W){
        if(e.size() == 3) continue;
        for(const auto& [u,w] : *W){
            if(w.size() == 3 || v == u) continue;

            pairs.push_back({v,u});
        }
    }
    // pick random pairs from the vector and add it with a chance of x/9 (where x is the product of (3-d_v) * (3-d_u))
    std::pair<int,int> pick;
    while(pairs.size() > 0){
        pick = pairs[get_bounded_random(pairs.size())];
        int chance = (3 - (*W)[pick.first].size()) * (3 - (*W)[pick.second].size());
        // try to insert, <chance>-times, with probability 1:9
        for(int i = 0; i < chance; i++){
            if(get_bounded_random(9) == 0){
                add_edge(W, pick.first, pick.second, get_bounded_random(max_cost));
                pairs.clear();
            }
        }
    }

    // check if the last two remaining vertices can be connected, otherwise restart graph generation
    int v = -1; int w = -1;
    for(const auto& [e,u] : *W){
        if(u.size() == 3) continue;
        else if(v == -1) v = e;
        else w = e;
    }
    if(v == -1 || w == -1) goto begin;

    // add last pair to W
    add_edge(W, v, w, get_bounded_random(max_cost));

    print_graph(W);

    // check if the graph was constructed correctly (TODO remove this)
    for(const auto& [v, e] : (*W)){
        if(e.size() != 3){
            std::cout << "Not deg3 !!" << std::endl;
            while(true){;}
        }
    }


    // // add remaining 4 points to a set
    // std::unordered_set<int> points = {L[0], L[1], L[2], L[3]};
    // // create a graph H, where the points form an edge that belong to different groups and thier groups are not yet connected
    // std::unordered_map<int, std::unordered_set<int>> H;
    // for(int i : points){
    //     for(int j : points){
    //         if(int(i/3)*3 == int(j/3)*3) continue; // check if they are in different groups
    //         // check if the group is already connected (TODO maybe something better?)
    //         for(int x = 0; x < 3; x++){
    //             for(int y = 0; y < 3; y++){
    //                 if(I[i+x] >= U && I[i+x]%2 == 0){ // check if point v+x is already selected and if its value is at an even place
    //                     if(L[I[i+x]+1] == j+y) goto already_connected;
    //                 }
    //                 else if(I[i+x] >= U && I[i+x]%2 == 1){ // check if point v+x is already selected and if its value is at an even place
    //                     if(L[I[i+x]-1] == j+y) goto already_connected;
    //                 }
    //             }
    //         }
    //         // i and j are suitable
    //         tmp = L[U-1]; L[U-1] = L[I[i]]; L[I[i]] = tmp; // swap L values
    //         I[tmp] = I[i]; I[i] = U-1; // swap I values
    //         tmp = L[U-2]; L[U-2] = L[I[j]]; L[I[j]] = tmp; // swap L values
    //         I[tmp] = I[j]; I[j] = U-2; // swap I values
    //         U -= 2;
    //
    //         goto last_two;
    //
    //     already_connected:;
    //     }
    // }
    // // no suitable point was found -> retry algorithm
    // goto begin;
    //
    // last_two:
    // // set last remaining points
    // int i = L[0]; int j = L[1];
    // // check if last two points are suitable, otherwise retry algorithm
    // if(int(i/3)*3 == int(j/3)*3) goto begin; // check if they are in different groups
    // // check if the group is already connected (TODO maybe something better?)
    // for(int x = 0; x < 3; x++){
    //     for(int y = 0; y < 3; y++){
    //         if(I[i+x] >= U && I[i+x]%2 == 0){ // check if point v+x is already selected and if its value is at an even place
    //             if(L[I[i+x]+1] == j+y) goto begin;
    //         }
    //         else if(I[i+x] >= U && I[i+x]%2 == 1){ // check if point v+x is already selected and if its value is at an even place
    //             if(L[I[i+x]-1] == j+y) goto begin;
    //         }
    //     }
    // }
    //
    // tmp = L[U-1]; L[U-1] = L[I[i]]; L[I[i]] = tmp; // swap L values
    // I[tmp] = I[i]; I[i] = U-1; // swap I values
    // tmp = L[U-2]; L[U-2] = L[I[j]]; L[I[j]] = tmp; // swap L values
    // I[tmp] = I[j]; I[j] = U-2; // swap I values
    // U -= 2;
    //
    // std::cout << "new values: U=" << U << "\nL: ";
    // for(int x = 0; x < n*3; x++){std::cout << L[x] << " ";} std::cout << "\nI: ";
    // for(int x = 0; x < n*3; x++){std::cout << I[x] << " ";} std::cout << std::endl;
    //
    // // construct the graph
    // for(int x = 0; x < n*3; x+=2){
    //     // get the groups of two connected points, connect the groups (vertices) and assign them a random value
    //     int v = int(L[x]/3);
    //     int u = int(L[x+1]/3);
    //
    //     add_edge(W, v, u, get_bounded_random(max_cost));
    // }
    //
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
