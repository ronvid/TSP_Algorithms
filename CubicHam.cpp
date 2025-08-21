#include <map>
#include <set>
#include <vector>
#include <functional>
#include <climits>
#include <iostream> //TODO debug only ig

#include "GraphUtility.hpp"

/**
 * This is a C++ version of David Eppsteins Implementation of Eppsteins Algorithm for finding
 * hamiltonian cycles and a solution to the Traveling Salesman Problem in Cubic Graph
 * TODO name changes
 *
 * NOTE changes
 * Vertices should have values 0+
 */

// NOTE TODO write that somewhere: actions that return none/nothing in EI, return false here

// function forward declarations
void remove(int v, int w);
void now_degree_two(int v);
bool safely_remove(int u, int w);
bool remove_third_leg(int v);
bool force(int v, int w);
bool force_into_triangle(int v, int w);
bool contract(int v);
bool handle_degree_two();
// extension
void handle_triangle();
void contract_triangle(int v, int w, int u);

void print_graph(std::map<int, std::map<int, bool>>* G);

// a copy of the given graph to run the algorithm on
std::map<int, std::map<int, bool>> G;

// a copy of the given weights to run the algorithm on
std::map<int, std::map<int, int>> W;

// keeps track of the weight of the current forced edges
int current_weight = 0;
// the weight of the current best weight graph
int min_found_weight = INT_MAX;

// a subgraph of forced edges in original G
std::map<int, std::map<int, bool>> forced_in_input;

// a subgraph of forced edges in current G
std::map<int, std::map<int, bool>> forced_in_current;

// set of vertices that are degree_two
std::vector<int> degree_two;

// set of triangles in the graph (triangles are represented as sets)
std::set<std::set<int>> triangles;

// set of all vertices that are adjacent to a forced edge
std::map<int, bool> forced_vertices;

// the action "stack"
std::vector<std::function<bool()>> actions = {};

int get_unforced_neighbour(int v);

std::function<bool()> main_ch = []{
    std::cout << "action: main" << std::endl;
    // main event dispatcher
    // returns true if a hamiltonian cycle was found, otherwise false

    if(degree_two.size() > 0){
        return handle_degree_two();
    }

    // check triangles
    if(triangles.size() > 0){
        handle_triangle();
        return false;
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
        std::cout << "action: continuation" << std::endl;
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

    return false;
};


bool ShortestHamiltonianCycle(std::map<int, std::map<int, bool>>* input,
                              std::map<int, std::map<int, int>>* input_weights,
                              std::map<int, std::map<int, bool>>* forced_edges, int* cost){

    // copy all values from the input graph to the graph used by the algorithm
    // copy weights from input weight graph
    // the forced edges graph is used to retun the forced edges, cost returns the cost
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
            // add weights to weight graph
            W[v][w] = (*input_weights)[v][w];
        }

        // inivtialize vertices in forced_in_input/current graphs
        forced_in_input[v];
        forced_in_current[v];

        // check if v is part of a triangle
        // create array with neighbours of v
        int a[G[v].size()]; int i = 0;
        for(const auto& [w, c] : G[v]){
            a[i] = w;
            i++;
        }
        // if v has only two neighbours -> only one triangle possible
        if(G[a[0]].contains(a[1])) triangles.insert({v, a[0], a[1]});
        if(G[v].size() == 3){
            if(G[a[1]].contains(a[2])) triangles.insert({v, a[1], a[2]});
            if(G[a[2]].contains(a[0])) triangles.insert({v, a[2], a[0]});
        }
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
            std::cout << "found cycle :D" << std::endl;
            cycle_found = true;
            std::cout << "Cost: " << current_weight << std::endl;
            // check if the found cycle has smaller cost than the best previous cycle
            if(current_weight < min_found_weight){
                min_found_weight = current_weight;
                delete_graph(forced_edges);
                copy_graph(&forced_in_input, forced_edges);
            }
        }
    }

    *cost = min_found_weight;
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

    std::function<bool()> unremove = [v, w, was_original, was_forced, weight]{
        std::cout << "action: unremove" << std::endl;
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
        std::cout << "action: not_degree_two" << std::endl;
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

    std::function<bool()> unforce = [v, w, was_original, v_not_previously_forced, w_not_previously_forced, weight]{
        std::cout << "action: unforce" << std::endl;
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

    return remove_third_leg(v) | remove_third_leg(w) |
        force_into_triangle(v,w) | force_into_triangle(v,w);
}

bool force_into_triangle(int v, int w){
    // after v-w was forced, check if w belongs to a triangle -> force opposite edge
    if(G[w].size() != 3){
        return true;
    }
    // get edges adjacent to w that are not v
    int x = -1;
    int y = -1;
    for(std::map<int, bool>::iterator z = G[w].begin(); z != G[w].end(); z++){
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
    for(std::map<int, bool>::iterator i = G[v].begin(); i != G[v].end(); i++){
        if(u == -1) u = (*i).first;
        else        w = (*i).first;
    }

    // check if parallel edge will be created
    if(G[u].contains(w)){
        // check if G is a triangle
        if(G.size() == 3){
            std::cout << " --> Tripple!" << std::endl;
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
        std::cout << "action: uncontract" << std::endl;
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
        std::cout << "action: unpop" << std::endl;
        degree_two.push_back(v);
        return false;
    };
    actions.push_back(unpop);

    return contract(v);
}

int get_unforced_neighbour(int v){ // TODO check if this functions is even neccassary
    // returns an unforced neighbour to v
    // NOTE not originaly a function in Eppsteins implementation
    for(std::map<int, bool>::iterator i = G[v].begin(); i != G[v].end(); i++){
        if(!forced_in_current[v].contains((*i).first)){
            return (*i).first;
        }
    }
    // TODO maybe assertion here
    return -1;
}

void handle_triangle(){
    std::cout << "Handle Triangle" << std::endl;
    // handles case that more than one triangle exists
    // returns true if cycle was found, otherwise false

    // remove triangle from list an assign its values to v, w, u
    std::set<int> tri = *--(triangles.end());
    triangles.erase(tri);
    int v = *tri.begin();
    int w = *++tri.begin();
    int u = *++++tri.begin();

    std::function<bool()> unerase = [tri]{
        triangles.insert(tri);
        return false;
    };
    actions.push_back(unerase);

    contract_triangle(v, w, u);
}

void contract_triangle(int v, int w, int u){
    std::cout << "Contract Triangle: " << v << " " << w << " " << u << std::endl;
    // remove the unforced edge between v, w, u with the highest cost

    // all three edges are forced -> no solution // TODO neccassary?
    if(forced_in_current[v].contains(w) && forced_in_current[w].contains(u) && forced_in_current[u].contains(v)){
        return;
    }
    int x, y;
    int c = INT_MIN;
    if(!forced_in_current[v].contains(w) && c < W[v][w]){
        x = v;
        y = w;
        c = W[v][w];
    }
    if(!forced_in_current[w].contains(u) && c < W[w][u]){
        x = w;
        y = u;
        c = W[w][u];
    }
    if(!forced_in_current[v].contains(w) && c < W[u][v]){
        x = u;
        y = v;
        c = W[u][v];
    }
    // check if x and y form a triangle with a different vertex
    for(const auto& [e, i] : G[x]){
        if(e != v && e != w && e != u && G[e].contains(y)){
            std::set<int> tri = {x,y,e};
            triangles.erase(tri);

            std::function<bool()> unerase_triangle = [tri]{
                triangles.insert(tri);
                return false;
            };
            actions.push_back(unerase_triangle);
        }
    }

    std::cout << "rem tri edge: " << x << "-" << y << std::endl;
    if(safely_remove(x, y)){
        actions.push_back(main_ch);
    }
}
