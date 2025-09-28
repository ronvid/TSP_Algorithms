#ifndef SCHUSTER_HPP
#define SCHUSTER_HPP

#include <map>
#include <set>
#include <vector>
#include <functional>
#include <climits>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "GraphUtility.hpp"

#include <stdexcept>
#include <iostream> // TODO remove

namespace Schuster{

/**
 * This is a C++ version of David Eppsteins Implementation of Eppsteins Algorithm for finding
 * hamiltonian cycles and a solution to the Traveling Salesman Problem in Cubic Graph
 *
 * Restrictions
 * Vertices should have values >= 0
 */


void remove(int v, int w);
void now_degree_two(int v);
bool safely_remove(int u, int w);
bool remove_third_leg(int v);
bool force(int v, int w);
bool force_into_triangle(int v, int w);
bool contract(int v);
bool handle_degree_two();

bool check_for_six_cycle(int v);
void remove_six_cycle_vertices(int v, int w);
    void remove_six_cycle_vertices(int v);

bool ShortestHamiltonianCycle(std::unordered_map<int, std::unordered_map<int, int>>* input_weights, std::unordered_map<int, std::unordered_map<int, bool>>* forced_edges, int* cost);

//void print_graph(std::map<int, std::map<int, bool>>* G);

// a copy of the given graph to run the algorithm on
std::unordered_map<int, std::unordered_map<int, bool>> G;

// a copy of the given weights to run the algorithm on
std::unordered_map<int, std::unordered_map<int, int>> W;

// keeps track of the weight of the current forced edges
int current_weight;
// the weight of the current best weight graph
int min_found_weight;

// a subgraph of forced edges in original G
std::unordered_map<int, std::unordered_map<int, bool>> forced_in_input;

// a subgraph of forced edges in current G
std::unordered_map<int, std::unordered_map<int, bool>> forced_in_current;

// set of vertices that are degree_two
std::vector<int> degree_two;

// set of all vertices that are adjacent to a forced edge
std::unordered_map<int, bool> forced_vertices;

// the action "stack"
std::vector<std::function<bool()>> actions;

int get_unforced_neighbour(int v);

// set of six cycles (represented as sets)
std::set<std::set<int>> six_cycles; // TODO mabe six cycle as unordered set


std::function<bool()> main_ch = []{
    // main event dispatcher
    // returns true if a hamiltonian cycle was found, otherwise false

    if(degree_two.size() > 0){
        return handle_degree_two();
    }

    // Now every vertex is degree three and forced edges form a matching
    // pick edge for rcursive search (optimaly one that is adjacent to a forced edge)
    int v = -1;
    if(forced_vertices.size() > 0){
        v = (*forced_vertices.begin()).first;
    }
    else{
        v = (*G.begin()).first;
    }

    int w = get_unforced_neighbour(v);

    std::function<bool()> continuation = [v,w]{
        // After searching first recursive subgraph
        if(force(v,w)){
            actions.push_back(main_ch);
        }
        return false;
    };

    actions.push_back(continuation);
    if(safely_remove(v,w)){
        actions.push_back(main_ch);
    }

    return false;
};


bool ShortestHamiltonianCycle(std::unordered_map<int, std::unordered_map<int, int>>* input_weights,
                              std::unordered_map<int, std::unordered_map<int, bool>>* forced_edges, int* cost){

    // initialize/reset used variables
    current_weight = 0;
    min_found_weight = INT_MAX;
    actions = {};

    G.clear();
    W.clear();
    forced_in_input.clear();
    forced_in_current.clear();
    degree_two.clear();
    forced_vertices.clear();
    six_cycles.clear();

    // copy all values from the input graph to the graph used by the algorithm
    // copy weights from input weight graph
    // the forced edges graph is used to retun the forced edges, cost returns the cost
    // add all degree two vertices to the degree_two set
    // check for any isolated/degree one vertices
    for(const auto& [v, e] : *input_weights){
        // vertex is isolated or degree one -> no hamiltonian cycle possible
        if(e.size() < 2){
            return false;
        }
        else if(e.size() == 2){
            degree_two.push_back(v);
        }
        for(const auto& [w, c] : e){
            // create edges in copy graph
            G[v][w] = true;
            // add weights to weight graph
            W[v][w] = c;
        }

        // inivtialize vertices in forced_in_input/current graphs
        forced_in_input[v];
        forced_in_current[v];
    }

    actions.push_back(main_ch);

    bool cycle_found = false; // was a cycle found?

    // the main backtracing loop
    std::function<bool()> a;
    while(actions.size() > 0){
        a = actions.back();
        actions.pop_back();
        if(a()){
            // hamiltonian cycle found
            cycle_found = true;

            // check if the found cycle has smaller cost than the best previous cycle
            if(current_weight < min_found_weight){
                min_found_weight = current_weight;
                delete_graph(forced_edges);
                copy_graph(&forced_in_input, forced_edges);
            }
        }
    }

    *cost = min_found_weight;
    std::cout << "Final sc count: " << six_cycles.size() << std::endl;
    return cycle_found;
}

void remove(int v, int w){
    // removes edge v-w from G
    bool was_original = G[v][w];
    G[v].erase(w);
    G[w].erase(v);
    bool was_forced = forced_in_current[v].contains(w);
    if (was_forced){
        forced_in_current[v].erase(w);
        forced_in_current[w].erase(v);
    }

    // remove weights
    int weight = W[v][w];
    W[v].erase(w);
    W[w].erase(v);

    // check if any of the vertices were part of a six cycle
    remove_six_cycle_vertices(v);
    remove_six_cycle_vertices(w);

    std::function<bool()> unremove = [v, w, was_original, was_forced, weight]{
        G[v][w] = G[w][v] = was_original;
        if(was_forced){
            forced_in_current[v][w] = forced_in_current[w][v] = true;
        }
        // add weights back
        W[v][w] = W[w][v] = weight;
        return false;
    };
    actions.push_back(unremove);

}

void now_degree_two(int v){
    // changing G caused v's degree to become two
    degree_two.push_back(v);

    std::function<bool()> not_degree_two = [v]{
        degree_two.pop_back();
        return false;
    };
    actions.push_back(not_degree_two);
}

bool safely_remove(int v, int w){
    // remove edge v-w and update degree data list
    // return true if successful, otherwise false
    if(forced_in_current[v].contains(w) || G[v].size() < 3 || G[w].size() < 3){
        return false;
    }
    remove(v,w);
    now_degree_two(v);
    now_degree_two(w);
    return true;
}

bool remove_third_leg(int v){
    // if v has two forced edges -> remove third unforced edge
    // returns true if successful, otherwise false
    if(G[v].size() != 3 || forced_in_current[v].size() != 2){
        return true;
    }
    int w = get_unforced_neighbour(v);
    if(G[w].size() <= 2){
        return false;
    }
    return safely_remove(v, w);
}

bool force(int v, int w){
    // add edge v-w to forced edges
    // return true if successful, otherwise false
    if(forced_in_current[v].contains(w)){
        return true; // already forced
    }
    if(forced_in_current[v].size() > 2 || forced_in_current[w].size() > 2){
        return false; // three incident forced edges
    }
    forced_in_current[v][w] = forced_in_current[w][v] = true;

    bool v_not_previously_forced = !forced_vertices.contains(v);
    bool w_not_previously_forced = !forced_vertices.contains(w);
    if(v_not_previously_forced) forced_vertices[v] = true;
    if(w_not_previously_forced) forced_vertices[w] = true;

    bool was_original = G[v][w];
    if(was_original){
        forced_in_input[v][w] = forced_in_input[w][v] = true;
    }

    // add weight
    int weight = W[v][w];
    current_weight += weight;

    // check if a six cycle edge was forced
    remove_six_cycle_vertices(v,w);

    // check if any of the forced vertices are part of a six cycle
    check_for_six_cycle(v);
    check_for_six_cycle(w);

    std::function<bool()> unforce = [v, w, was_original, v_not_previously_forced, w_not_previously_forced, weight]{
        if(v_not_previously_forced) forced_vertices.erase(v);
        if(w_not_previously_forced) forced_vertices.erase(w);

        forced_in_current[v].erase(w);
        forced_in_current[w].erase(v);
        if(was_original){
            forced_in_input[v].erase(w);
            forced_in_input[w].erase(v);
        }
        // remove weight
        current_weight -= weight;

        return false;
    };
    actions.push_back(unforce);

    return remove_third_leg(v) && remove_third_leg(w) &&
        force_into_triangle(v,w) && force_into_triangle(w,v);
}

bool force_into_triangle(int v, int w){
    // after v-w was forced, check if w belongs to a triangle -> force opposite edge
    if(G[w].size() != 3){
        return true;
    }
    // get edges adjacent to w that are not v
    int x = -1;
    int y = -1;
    for(std::unordered_map<int, bool>::iterator z = G[w].begin(); z != G[w].end(); z++){
        if((*z).first != v && x == -1) x = (*z).first;
        else if((*z).first != v && y == -1) y = (*z).first;
    }

    if(!G[x].contains(y)){
        return true;
    }
    return force(x,y);
}

bool contract(int v){
    // remove degree two vertex v
    // returns true if cycle should be reported, otherwise false
    // appends recursive search of contracted graph to action stack

    // get adjacent vertices of v
    int u = -1;
    int w = -1;
    for(std::unordered_map<int, bool>::iterator i = G[v].begin(); i != G[v].end(); i++){
        if(u == -1) u = (*i).first;
        else        w = (*i).first;
    }

    // check if parallel edge will be created
    if(G[u].contains(w)){
        // check if G is a triangle
        if(G.size() == 3){
            return force(u,v) && force(v,w) && force(u,w);
        }
        if (!safely_remove(u,w)){
            return false;
        }
    }

    if(!force(u,v) | !force(v,w)){
        return false;
    }
    int new_weight = W[v][u] + W[v][w]; // get weight of contracted edges

    remove(u,v);
    remove(v,w);
    G[u][w] = G[w][u] = false;
    forced_in_current[u][w] = forced_in_current[w][u] = true;
    G.erase(v);
    forced_vertices.erase(v);

    // change weights
    W[u][w] = W[w][u] = new_weight;
    W.erase(v);

    std::function<bool()> uncontract = [v, u, w]{
        G[u].erase(w);
        G[w].erase(u);
        forced_in_current[u].erase(w);
        forced_in_current[w].erase(u);
        forced_vertices[v] = true;
        G[v];

        // uncontract weights graph
        W[u].erase(w);
        W[w].erase(u);
        W[v];

        return false;
    };
    actions.push_back(uncontract);
    actions.push_back(main_ch);

    return false;
}

bool handle_degree_two(){
    // handles case that degree two vertices exist
    // return true if cycle was found, otherwise false

    int v = degree_two.back();
    degree_two.pop_back();

    std::function<bool()> unpop = [v]{
        degree_two.push_back(v);
        return false;
    };
    actions.push_back(unpop);

    return contract(v);
}

int get_unforced_neighbour(int v){
    // returns an unforced neighbour to v
    // not originaly a function in Eppsteins implementation
    for(std::unordered_map<int, bool>::iterator i = G[v].begin(); i != G[v].end(); i++){
        if(!forced_in_current[v].contains((*i).first)){
            return (*i).first;
        }
    }
    // TODO maybe assertion here
    print_graph(&forced_in_current);
    std::cout << " ----- " << std::endl;
    print_graph(&G);
    throw std::invalid_argument( " unforced neighbour returned -1"); // TODO remove or something
    return -1;
}

bool check_for_six_cycle(int v){
    //std::cout << "check sc: " << v << std::endl;
    // check if v is part of a six cycle

    // v needs to have one forced edge and two unorced edges
    if(G[v].size() != 3) return false;
    int w = -1;
    int u = -1;
    for(const auto& [e, c] : G[v]){
        if(w == -1 && !forced_in_current[v].contains(e)) w = e;
        else if(u == -1 && !forced_in_current[v].contains(e)) u = e;
        else if(!forced_in_current[v].contains(e)) return false; // found a third unforced edges
    }
    if(w == -1 || u == -1) return false; // v has less than 2 unforced edges

    // construct six cycle
    for(const auto& [x, c_x] : G[w]){
        if(x == v || x == u || forced_in_current[w].contains(x)) continue;

        for(const auto& [y, c_y] : G[x]){
            if(y == w || y == v || y == w || forced_in_current[x].contains(y)) continue;

            for(const auto& [z, c_z] : G[y]){
                if(z == x || z == w || z == v || z == u || forced_in_current[y].contains(z)) continue;

                // check if z and u are unforced neighbours -> six cycle found
                if(u != y && G[z].contains(u) && !forced_in_current[z].contains(u)){
                    //std::cout << "SC found: " << v << " " << w << " " << x << " " << y << " " << z << " " << u << std::endl;
                    // try to insert six cycle to list
                    std::set<int> sc = {v,w,u,x,y,z};
                    if(sc.size() != 6) continue; // should not be nessaccary (since checks in conditionals) (maybe check whats better TODO)
                    if(six_cycles.insert(sc).second){
                        // if six cycle can be inserted
                        //std::cout << "SC inserted: " << v << " " << w << " " << x << " " << y << " " << z << " " << u << std::endl;
                        std::function<bool()> uninsert_six_cycle = [sc]{
                            //std::cout << "uninsert " << six_cycles.size() << ": "<< std::endl;
                            //for(int i : sc) std::cout << i << " ";
                            //std::cout << std::endl;
                            six_cycles.erase(sc);
                            return false;
                        };
                        actions.push_back(uninsert_six_cycle);
                    }
                }
            }
        }
    }
    return false;
}

void remove_six_cycle_vertices(int v, int w){
    //std::cout << "remove sc: " << v << " " << w << std::endl;
    // check if both given vertices are part of a six cycle, remove six_cycle if they are

    // worst case: runs size * (deleted elements +1) times, maybe find a better way TODO
    for(std::set<std::set<int>>::iterator sc = six_cycles.begin(); sc != six_cycles.end();){
        //std::cout << "it " << six_cycles.size() << std::endl;
        if((*(sc)).contains(v) && (*(sc)).contains(w)){
            //std::cout << "remove!" << std::endl;
            six_cycles.erase(sc);

            //std::cout << "removed!" << std::endl;

            std::set<int> p = *sc; // give object instead of iterator to the lambda function
            std::function<bool()> unremove_six_cycle = [p]{
                //std::cout << "unremove " << six_cycles.size() << ": " << std::endl;
                //For(int i : (p)) std::cout << i << " ";
                six_cycles.insert(p);
                //std::cout << std::endl;
                return false;
            };
            actions.push_back(unremove_six_cycle);

            sc = six_cycles.begin();
            if(six_cycles.size() == 0) break;
        }
        else{
            //std::cout << "inc" << std::endl;
            sc++;
        }
    }
    //std::cout << "done!" << std::endl;
}

void remove_six_cycle_vertices(int v){
    //std::cout << "remove sc: " << v << std::endl;
    // check if the given vertex is part of a six cycle, remove six_cycle if it is

    // worst case: runs size * (deleted elements +1) times, maybe find a better way TODO https://en.cppreference.com/w/cpp/container/set/erase.html
    for(std::set<std::set<int>>::iterator sc = six_cycles.begin(); sc != six_cycles.end();){
        //std::cout << "it " << six_cycles.size() << std::endl;
        if((*(sc)).contains(v)){
            //std::cout << "remove!" << std::endl;
            six_cycles.erase(sc);

            //std::cout << "removed!" << std::endl;
            std::set<int> p = *sc; // give object instead of iterator to the lambda function
            std::function<bool()> unremove_six_cycle = [p]{
                //std::cout << "unremove " << six_cycles.size() << ": " << std::endl;
                //for(int i : (p)) std::cout << i << " ";
                six_cycles.insert(p);
                //std::cout << std::endl;
                return false;
            };
            actions.push_back(unremove_six_cycle);

            sc = six_cycles.begin();
            if(six_cycles.size() == 0) break;
        }
        else{
            //std::cout << "inc" << std::endl;
            sc++;
        }
    }
    //std::cout << "done!" << std::endl;
}

}

#endif
