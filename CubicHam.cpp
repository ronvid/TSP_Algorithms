#include <map>
#include <set>
#include <vector>
#include <functional>
#include <iostream> //TODO debug only ig

/**
 * This is a C++ version of David Eppsteins Implementation of Eppsteins Algorithm for finding
 * hamiltonian cycles and a solution to the Traveling Salesman Problem in Cubic Graph
 * TODO name changes
 *
 * NOTE changes
 */

// NOTE TODO write that somewhere: actions that return none/nothing in EI, return false here

// function forward declarations
void remove(int v, int w);
void now_degree_two(int v);
bool safely_remove(int u, int w);
bool remove_third_leg(int v);
bool force(int v, int w);
bool force_into_triangle(int v, int w);
bool contract(int v); // NOTE returns false instead of none
bool handle_degree_two();

// a copy of the given graph to run the algorithm on
std::map<int, std::map<int, bool>> G;

// a subgraph of forced edges in original G
std::map<int, std::map<int, bool>> forced_in_input;

// a subgraph of forced edges in current G
std::map<int, std::map<int, bool>> forced_in_current;

// set of vertices that are degree_two
std::vector<int> degree_two;

// set of all vertices that are adjacent to a forced edge
std::map<int, bool> forced_vertices;

// the action "stack"
std::vector<std::function<bool()>> actions = {};

int get_unforced_neighbour(int v);

std::function<bool()> main_ch = []{
    // main event dispatcher
    // returns true if a hamiltonian cycle was found, otherwise false

    if(degree_two.size() > 0){
        return handle_degree_two();
    }

    // Now every vertex is degree three and forced edges form a matching
    // pick edge for rcursive search (optimaly one that is adjacent to a forced edge)
    int v;
    if(forced_vertices.size() > 0){
        // takes first vertex from forced vertices
        v = (*forced_vertices.begin()).first;
    }
    else{
        // takes first vertex from graph
        v = (*G.begin()).first;
    }

    int w = get_unforced_neighbour(v);

    std::function<bool()> continuation = [v,w]{
        // After searching first recursive subgraph
        if(force(v,w)){
            actions.push_back(main_ch);
        }
        return false; // NOTE this should do nothing
    };

    actions.push_back(continuation);
    if(safely_remove(v,w)){
        actions.push_back(main_ch);
    }

    return true; // TODO TEMP!
};


bool ShortestHamiltonianCycle(std::map<int, std::map<int, bool>>* input){

    // copy all values from the input graph to the graph used by the algorithm
    // add all degree two vertices to the degree_two set
    // check for any isolated/degree one vertices
    for(const auto& [v, e] : *input){
        // vertex is isolated or degree one -> no hamiltonian cycle possible
        if(e.size() < 2){
            return false;
        }
        else if(e.size() == 2){
            degree_two.push_back(v);
        }
        for(const auto& [w, og] : e){
            // create edges in copy graph
            G[v][w] = true;
        }

        // inivtialize vertices in forced_in_input/current graphs
        forced_in_input[v];
        forced_in_current[v];
    }

    actions.push_back(main_ch);

    // the main backtracing loop
    std::function<bool()> a;
    while(actions.size() > 0){
        a = actions.back();
        actions.pop_back();
        if(a()){
            // hamiltonian cycle found
            // TODO do stuff with found ham cycle
            std::cout << "found cycle :D" << std::endl;
        }
    }

    return true; // TODO remove ig
}

// TODO all of this is TMP
void remove(int v, int w){
    // removes edge v-w from G
    bool was_original = G[v][w];
    G[v].erase(w);
    G[w].erase(v);
    bool was_forced = forced_in_current[v].contains(w);

};
void now_degree_two(int v){};
bool safely_remove(int u, int w){return false;};
bool remove_third_leg(int v){return false;};
bool force(int v, int w){return false;};
bool force_into_triangle(int v, int w){return false;};
bool contract(int v){return false;}; // NOTE returns false instead of none
bool handle_degree_two(){return false;};

int get_unforced_neighbour(int v){ // TODO check if this functions is even neccassary
    // returns an unforced neighbour to v
    // NOTE not originaly a function in Eppsteins implementation
    for(std::map<int, bool>::iterator i = G[v].begin(); i != G[v].end(); i++){
        if(!forced_in_current[v].contains((*i).first)){
            std::cout << "found: " << v << " " << (*i).first << std::endl;
            return (*i).first;
        }
    }
    // TODO maybe assertion here
    return -1;
}
