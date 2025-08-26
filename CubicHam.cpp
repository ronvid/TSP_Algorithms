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
void remove_triangle(int v, int w, int u);
void add_triangle(int v, int w, int u);
bool handle_triangle();
bool contract_triangle(int v, int w, int u);
void check_for_four_cycle(int v);
void remove_four_cycle(std::set<int> fc);
bool solve_four_cycle();
void check_for_live_six_cycle(int v);
void erase_live_six_cycle(std::set<int> sc);

void print_graph(std::map<int, std::map<int, bool>>* G);

// a copy of the given graph to run the algorithm on
std::map<int, std::map<int, bool>> G;

// a copy of the given weights to run the algorithm on
std::map<int, std::map<int, int>> W;

// keeps track of the weight of the current forced edges
int current_weight = 0;
// the weight of the current best weight graph
int min_found_weight = INT_MAX;

// determines the number assigned to a new vertex
int next_vertex_id = 0;

// a subgraph of forced edges in original G
std::map<int, std::map<int, bool>> forced_in_input;

// a subgraph of forced edges in current G
std::map<int, std::map<int, bool>> forced_in_current;

// set of vertices that are degree_two
std::vector<int> degree_two;

// set of triangles in the graph (triangles are represented as sets)
std::set<std::set<int>> triangles;
// set of 4-cycles simmilar to triangles
std::set<std::set<int>> four_cycles;
// subset of four cycles, where not every neighbour is forced
std::set<std::set<int>> unforced_four_cylces;
// set of live six cycles (six cycles with no forced edges and at least one forced neighbour)
std::set<std::set<int>> live_six_cycles;

// set of all vertices that are adjacent to a forced edge
std::map<int, bool> forced_vertices;

// the action "stack"
std::vector<std::function<bool()>> actions = {};

int get_unforced_neighbour(int v);

std::function<bool()> main_ch = []{
    std::cout << "action: main" << std::endl;
    // main event dispatcher
    // returns true if a hamiltonian cycle was found, otherwise false

    // Step 1
    if(degree_two.size() > 0){
        return handle_degree_two();
    }

    // check triangles
    if(triangles.size() > 0){
        return handle_triangle();
    }

    // Step 2
    // if forced 4-cycles form a collection of disjoint 4-cycles, and all 4-cycles are fully forced
    // every vertex is part of exactly one 4-cycle
    if(G.size() == four_cycles.size() * 4 && unforced_four_cylces.size() == 0){
        return solve_four_cycle();
    }

    // Step 3
    std::cout << " --- Step 3 --- " << std::endl;
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
        if(G[a[0]].contains(a[1])) add_triangle(v, a[0], a[1]);
        if(G[v].size() == 3){
            if(G[a[1]].contains(a[2])) add_triangle(v, a[1], a[2]);
            if(G[a[2]].contains(a[0])) add_triangle(v, a[2], a[0]);
        }

        // maintain vertex id
        if(v >= next_vertex_id){
            next_vertex_id = v + 1;
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
    std::cout << "remove: " << v << "-" << w << std::endl;

    // check if v-w form triangles -> remove them
    for(const auto& [e, c] : G[v]){
        if(G[w].contains(e)){
            remove_triangle(v,w,e);
        }
    }
    // check if v or w is part of a four cycle -> remove four cycle
    for(std::set<int> fc : four_cycles){
        if(fc.contains(v) || fc.contains(w)){
            remove_four_cycle(fc);
            break;
        }
    }
    // check if v-w is part of a live six cycle -> remove cycle
    for(std::set<int> sc : live_six_cycles){
        if(sc.contains(v) && sc.contains(w)){
            erase_live_six_cycle(sc);
        }
    }

    // removes edge v-w from G
    bool was_original = G[v][w];
    G[v].erase(w);
    G[w].erase(v);
    bool was_forced = forced_in_current[v].contains(w);
    if (was_forced){
        forced_in_current[v].erase(w);
        forced_in_current[w].erase(v);
        current_weight -= W[v][w]; // reduce total cost of forced edges
    }

    // remove weights
    int weight = W[v][w];
    W[v].erase(w);
    W[w].erase(v);

    std::function<bool()> unremove = [v, w, was_original, was_forced, weight]{
        std::cout << "action: unremove" << std::endl;
        // add weights back
        W[v][w] = W[w][v] = weight;

        G[v][w] = G[w][v] = was_original;
        if(was_forced){
            forced_in_current[v][w] = forced_in_current[w][v] = true;
            current_weight += W[v][w];
        }
        return false;
    };
    actions.push_back(unremove);

}

void now_degree_two(int v){
    std::cout << "now deg 2: " << v << std::endl;
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
    std::cout << "safely remove: " << v << "-" << w << std::endl;
    // remove edge v-w and update degree data list and triangle list
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
    std::cout << "remove third leg: " << v << std::endl;
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
    std::cout << "force: " << v << "-" << w << std::endl;
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

    // if the forced edge lies within a four cycle remove it from the list
    for(std::set<int> fc : four_cycles){
        if(fc.contains(v) && fc.contains(w)){
            remove_four_cycle(fc);
            break;
        }
    }
    // if one vertex is part of an unforced 4-cycle -> remove fc so it can be added as forced fc
    for(std::set<std::set<int>>::iterator fc = unforced_four_cylces.begin(); fc != unforced_four_cylces.end();){
        if((*fc).contains(v) || (*fc).contains(w)){
            remove_four_cycle(*fc);
            fc = unforced_four_cylces.begin(); // hack to avoid seg-fault (worst case iterate the list 3 times) TODO better way for this?
        }
        else{
            fc++;
        }
    }

    // look if the newly forced vertices create a four cycle
    check_for_four_cycle(v);
    check_for_four_cycle(w);

    // check if v and w are both part of a live six cycle -> remove six cycle from list
    for(std::set<std::set<int>>::iterator sc = live_six_cycles.begin(); sc != live_six_cycles.end();){
        if((*sc).contains(v) && (*sc).contains(w)){
            erase_live_six_cycle(*sc);
            sc = live_six_cycles.begin();
        }
        else{
            sc++; // simmilar hack to 4-cycle checking TODO better way?
        }
    }

    // look if the newly forced vertices create a live six cycle
    check_for_live_six_cycle(v);
    check_for_live_six_cycle(w);

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
        force_into_triangle(v,w) | force_into_triangle(w,v);
}

bool force_into_triangle(int v, int w){
    std::cout << "force into triangle: " << v << "-" << w << std::endl;
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
    std::cout << "contract: " << v << std::endl;
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

    // check if parallel edge will be created (v-w-u forms triangle)
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
    current_weight += W[u][w]; // add weight of new edge to total weight

    // TODO add a triangle here if v-w-u-x form a 4-c (maybe?)
    // insert tri if was formed
    for(const auto& [e,c] : G[w]){
        if(G[u].contains(e)){
            add_triangle(w, u, e);
        }
    }

    // check if u and w are 4-cycle edges
    check_for_four_cycle(u);
    check_for_four_cycle(w);

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
    std::cout << "handle deg 2" << std::endl;
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

bool handle_triangle(){
    std::cout << "Handle Triangle" << std::endl;
    // handles case that more than one triangle exists
    // returns true if cycle was found, otherwise false

    // remove triangle from list an assign its values to v, w, u
    std::set<int> tri = *--(triangles.end());
    triangles.erase(tri);
    int v = *tri.begin();
    int w = *++tri.begin();
    int u = *++++tri.begin();

    return contract_triangle(v, w, u);
}

bool contract_triangle(int v, int w, int u){
    std::cout << "Contract Triangle: " << v << " " << w << " " << u << std::endl;
    // removes a triangle from the graph and adds a single vertex in its position

    // save neighbouring vertices
    int a, b, c;
    for(const auto& [e, o] : G[v]){ if(e != w && e != u){ a = e; break; }}
    for(const auto& [e, o] : G[w]){ if(e != v && e != u){ b = e; break; }}
    for(const auto& [e, o] : G[u]){ if(e != v && e != w){ c = e; break; }}

    bool f_a = false; bool f_b = false; bool f_c = false; // saves if edes x-a, x-b, x-c should be forced

    // save costs of edges TODO nicer way to save this?
    int vw = W[v][w]; int wu = W[w][u]; int uv = W[u][v];
    int va = W[v][a]; int wb = W[w][b]; int uc = W[u][c];

    // if a, b and c are the same -> solve directly
    if(a == b && b == c){
        if(G.size() != 4){ // G contains independent(?) groups -> no solution
            return false;
        }
        else{
            std::cout << "solve directly!" << va + wu << " " << wb + uv << " " <<  uc + vw << " " << std::endl;
            // remove edge of a that has the highest cost and let deg_2 handle the rest
            // if it can be removed is is not false, if not possible, check others
            if(va + wu >= wb + uv && va + wu >= uc + vw && safely_remove(v,a)){
                actions.push_back(main_ch);
            }
            else if(wb + uv >= va + wu && wb + uv >= uc + vw && safely_remove(w,b)){
                actions.push_back(main_ch);
            }
            else if(uc + vw >= va + wu && uc + vw >= wb + uv && safely_remove(w,c)){
                actions.push_back(main_ch);
            }
            // if none could be removed -> no solution
            return false;
        }
    }

    // TODO re check this part (if correct (esp. formulars) and maybe if a better way exists) {
    // add new edge x
    int x = next_vertex_id++;
    G[x];
    // insert edge x-a
    G[x][a] = G[a][x] = false;
    W[x][a] = W[a][x] = va + wu;
    if(forced_in_current[v].contains(a)){ // if v-a was forced -> force x-a
        f_a = true;
    }

    // insert edge x-b, check if it already exists
    if(G[x].contains(b)){ // a and b are the same
        if(!forced_in_current[x].contains(b)){ // previous edge was not forced use x-b if forced or edge with lower cost
            if(forced_in_current[w].contains(b)){ // overwrite if wb was forced
                W[x][b] = wb + uv;
                f_b = true;
            }
            else if(wb + uv < W[x][b]){ // overwrite if wb has lower cost
                W[x][b] = wb + uv;
            }
        }
        // since a and be are the same -> x and b are now only deg 2
        now_degree_two(x);
        now_degree_two(b);
    }
    else{
        G[x][b] = G[b][x] = false;
        W[x][b] = W[b][x] = wb + uv;
        if(forced_in_current[w].contains(b)){
            f_b = true;
        }
    }

    // insert edge x-c, check if it already exists
    if(G[x].contains(c)){ // a/b and c are the same
        if(!forced_in_current[x].contains(c)){ // previous edge was not forced use x-c if forced or edge with lower cost
            if(forced_in_current[u].contains(c)){ // overwrite if wb was forced
                W[x][c] = uc + vw;
                f_c = true;
            }
            else if(uc + vw < W[x][c]){ // overwrite if uc has lower cost
                W[x][c] = uc + vw;
            }
        }
        // since a and be are the same -> x and c are now only deg 2
        now_degree_two(x);
        now_degree_two(c);
    }
    else{
        G[x][c] = G[c][x] = false;
        W[x][c] = W[c][x] = uc + vw;
        if(forced_in_current[u].contains(c)){
            f_c = true;
        }
    }
    // TODO }

    // erase old edges and costs
    remove(v,w);
    remove(v,a);
    remove(w,u);
    remove(w,b);
    remove(u,v);
    remove(u,c);
    G.erase(v); W.erase(v);
    G.erase(w); W.erase(w);
    G.erase(u); W.erase(u);
    // force edges of x that need to be forced
    if(f_a) force(x,a);
    if(f_b) force(x,b);
    if(f_c) force(x,c);

    std::function<bool()> uncontract_triangle = [v,w,u,x,a,b,c]{
        /// insert edges back to G and W
        G[v]; W[v];
        G[w]; W[w];
        G[u]; W[u];
        // remove new edges x-a x-b x-c
        G.erase(x); W.erase(x);
        G[a].erase(x); G[b].erase(x); G[c].erase(x);
        W[a].erase(x); W[b].erase(x); W[c].erase(x);
        return false;
    };
    actions.push_back(uncontract_triangle);
    actions.push_back(main_ch);

    return false;
}

void remove_triangle(int v, int w, int u){
    std::cout << "remove triangle: " << v << "-" << w << "-" << u << std::endl;
    std::set<int> tri = {v, w, u};
    triangles.erase(tri);

    std::function<bool()> unremove_triangle = [tri]{
        triangles.insert(tri);
        return false;
    };
    actions.push_back(unremove_triangle);
}

void add_triangle(int v, int w, int u){
    std::cout << "add triangle: " << v << "-" << w << "-" << u << std::endl;
    std::set<int> tri = {v,w,u};
    triangles.insert(tri);

    std::function<bool()> unadd_triangle = [tri]{
        triangles.erase(tri);
        return false;
    };
    actions.push_back(unadd_triangle);
}

void check_for_four_cycle(int v){
    std::cout << "check for cycle " << v << std::endl;
    // check if v and any other forced vertex form a 4-cycle
    // check if v can be a 4-cycle edge / is not already a 4-cycle edge
    if(G[v].size() != 3) return;
    for(std::set<int> c : four_cycles){
        if(c.contains(v)) return;
    }
    // check if v has two unforced edges, that have three edges
    int w = -1; int u = -1;
    for(const auto& [e, o] : G[v]){
        if(forced_in_current[v].contains(e) || G[e].size() != 3) continue;
        if(w == -1) w = e;
        else u = e;
    }
    if(u == -1) return; // more than one forced edge or w and u don't have three edges

    // check if w and u have a common vertex (that is not v) (w-x and u-x are unforced)
    int x = -1;
    for(const auto& [e, o] : G[w]){
        if(forced_in_current[w].contains(e) || forced_in_current[u].contains(e)) continue;
        if(e != v && G[u].contains(e)){ x = e; break;}
    }
    if(x == -1) return; // w and u dont share an unforced edge

    std::set<int> fc = {v,w,u,x};
    bool unforced = false;
    // if v and x are forced -> force neighbouring edges of w and u and add to four cycles
    if(forced_vertices.contains(v) && forced_vertices.contains(x)){
        std::cout << "add forced fc: " << v << " " << w << " " << u << " " << x << std::endl;
        four_cycles.insert(fc);
        //force neighbours of w and u
        for(const auto& [e, o] : G[w]){
            if(e != v && e != x){ force(w,e); break;}
        }
        for(const auto& [e, o] : G[u]){
            if(e != v && e != x){ force(u,e); break;}
        }
    }
    // if both w and u are forced -> was alread a four cycle TODO remove?
    // check if w or u are forced -> add 4-cycle to unforced 4-cycles
    else if(forced_vertices.contains(w) || forced_vertices.contains(u)){
        std::cout << "add unforced fc: " << v << " " << w << " " << u << " " << x << std::endl;
        four_cycles.insert(fc);
        unforced_four_cylces.insert(fc);
        unforced = true;
    }
    // neither w, u or x are forced, no viable 4-cycle
    else{
        return;
    }

    std::function<bool()> erase_four_cycle = [fc, unforced]{
        four_cycles.erase(fc);
        if(unforced) unforced_four_cylces.erase(fc);
        return false;
    };
    actions.push_back(erase_four_cycle);
}

void remove_four_cycle(std::set<int> fc){
    std::cout << "remove four cycle: " << *fc.begin() << " " << *++fc.begin() << " " << *++++fc.begin() << " " << *++++++fc.begin() << std::endl;
    four_cycles.erase(fc);

    bool unforced = false;
    if(unforced_four_cylces.contains(fc)){
        unforced = true;
        unforced_four_cylces.erase(fc);
    }

    std::function<bool()> unremove_fc = [fc, unforced]{
        four_cycles.insert(fc);
        if(unforced) unforced_four_cylces.insert(fc);
        return false;
    };
    actions.push_back(unremove_fc);
}

bool solve_four_cycle(){
    // Step 2 in Eppstein's algorithm
    // NOTE uses a workaround, were costs are calculated but edges are not forced (doesn't matter currently, since the triangle removing, has a simmilar problem, but once/if that is fixed, this should be fixed as well) TODO
    std::cout << "solve four cycle" << std::endl;

    // the graph structures for creating the minimum spanning tree
    std::map<int, std::map<int, int>> mst; // mnimum spanning tree with weights
    std::map<int, int> vertex_to_mst; // vertices are mapped to thier corresponding vertex in the mst
    int id = 0; // the next id for the mst
    int calculated_cost = 0; // the additional cost calculated by the fc-solver (workaround)

    // H consists of the 4-cycle edges, that are opposite to each other and have combined lower costs,
    // than the other opposite edges of the same 4-cycle
    std::set<std::set<int>> H;
    for(std::set<int> fc : four_cycles){
        // a-d represent the four vertices of the 4-cycle, a and c are opposite and a and d between
        int a,c,d; int b = -1;
        a = *(fc.begin());
        for(int i : fc){ // opposite of a is either not connected to a or connected by a forced edge
            if(a != i && (!G[a].contains(i) || forced_in_current[a].contains(i))){ c = i; break; }
        }
        for(const auto& [e, o] : G[a]){
            if(forced_in_current[a].contains(e)) continue;
            if(b == -1) b = e;
            else d = e;
        }
        // add the edges to H, that have a smaller combined weight
        if(W[a][b] + W[c][d] < W[a][d] + W[c][b]){
            H.insert({a,b});
            H.insert({c,d});
            // create an edge in the mst, with cost
            int cost = W[a][d] + W[b][c] - (W[a][b] + W[c][d]); // cost of this H edge pair
            mst[id][id+1] = cost;
            mst[id+1][id] = cost;
            calculated_cost += W[a][b] + W[c][d]; // workaround
            // map the vertices of G to the vertices of the mst
            vertex_to_mst[a] = id; vertex_to_mst[b] = id;
            vertex_to_mst[c] = id+1; vertex_to_mst[d] = id+1;
            id += 2;
        }
        else{
            H.insert({a,d});
            H.insert({c,b});
            // create an edge in the mst, with cost
            int cost = W[a][b] + W[d][c] - (W[a][d] + W[b][c]);
            mst[id][id+1] = cost;
            mst[id+1][id] = cost;
            calculated_cost += W[a][d] + W[b][c]; // workaround
            // map the vertices of G to the vertices of the mst
            vertex_to_mst[a] = id; vertex_to_mst[d] = id;
            vertex_to_mst[b] = id+1; vertex_to_mst[c] = id+1;
            id += 2;
        }

    }

    // combine the vertices that should be one vertex
    // every vertex on one path should be combined to one vertex
    int start = (*vertex_to_mst.begin()).first; // start of the path
    int cur = start; // current vertex in the path
    int mst_cur = vertex_to_mst[cur]; // current vertex of mst to add to
    while(true){
        // alternate between H edges and forced edges
        // find a vertex to form the H edge
        for(const auto& [w, o] : G[cur]){
            if (H.contains({cur, w})) {cur = w; break; }
        }
        // remove new cur vertex from the map
        vertex_to_mst.erase(cur);

        // find vertex to form forced ege with (should only have one forced edge)
        cur = (*forced_in_current[cur].begin()).first;
        if(cur == start){
            // the new vertex is the start of the path -> end iteration
            vertex_to_mst.erase(cur);
            // if the map is empty the mst was created, if not empty continue with the next vertex
            if(vertex_to_mst.size() == 0){
                break;
            }
            start = (*vertex_to_mst.begin()).first;
            cur = start;
            mst_cur = vertex_to_mst[start];
            continue;

        }
        int mst_v = vertex_to_mst[cur]; // mst vertex of the current vertex
        int mst_w = (*mst[mst_v].begin()).first; // mst vertex connected to v (should only exist one)
        // add mst w to the cur mst vertex and update mst w, delete mst v
        // if mst v and w are the same, only delete them (no self loops)
        if(mst_cur != mst_w){
            // if an edge already between cur and w, only change cost if new cost would be smaller
            if(!mst[mst_cur].contains(mst_w) || mst[mst_v][mst_w] < mst[mst_cur][mst_w]){
                mst[mst_cur][mst_w] = mst[mst_v][mst_w];
                mst[mst_w][mst_cur] = mst[mst_v][mst_w];
            }
        }
        mst[mst_w].erase(mst_v);
        mst.erase(mst_v);

        // erase vertex from map
        vertex_to_mst.erase(cur);

    }
    // now we have the graph structure to construct the minimum spanning tree on
    std::set<int> explored = { (*mst.begin()).first }; // set of mst vetices that are already in the mst
    int min_cost = INT_MAX; // cost of the edge with the smallest cost
    int min_vertex = -1; // edge with smallest cost
    while(explored.size() != mst.size()){
        // check every edge of the explored vertices and add the unexplored vertex with the snmallest cost
        for(int v : explored){
            for(const auto& [w, cost] : mst[v]){
                if(cost < min_cost && !explored.contains(w)){ // check if the new edge is smaller and w is not already explored
                    min_cost = cost;
                    min_vertex = w;
                }
            }
        }
        // check if a vertex was found (if none found -> disjointed graph, no solution)
        if(min_vertex == -1){
            return false;
        }
        //std::cout << "add: " << min_vertex << " with: " << min_cost << "e.size: " << explored.size() << std::endl;
        // if vertex was found, add it to explored, force non H edges and reset iteratoin values
        explored.insert(min_vertex);
        // increase cost
        calculated_cost += min_cost; // workaround

        min_cost = INT_MAX;
        min_vertex = -1;
    }

    // cost of the mst was calculated, add it to the cost of the graph
    current_weight += calculated_cost; // workaround

    // function part of the workaround
    std::function<bool()> subtract_cost = [calculated_cost]{
        current_weight -= calculated_cost;
        return false;
    };
    actions.push_back(subtract_cost);

    // debug print
    std::cout << " Calculated Cost: " << calculated_cost << std::endl;

    std::cout << " - AFTER -" << std::endl;
    std::cout << "MST" << std::endl;
    for(const auto& [v, w] : mst){
        std::cout << v << ": ";
        for(const auto& [e, cost] : w){
            std::cout << e << "." << cost << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "vertex to mst" << std::endl;
    for(const auto& [v, w] : vertex_to_mst){
        std::cout << v << ":" << w << std::endl;
    }


    std::cout << "H values" << std::endl;
    for(std::set<int> h : H){
        for(int i : h){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    return true;
}

void check_for_live_six_cycle(int v){
    std::cout << "check for six cycle: " << v << std::endl;
    // checks if v is part of a live six cycle
    // check if v has 3 neighbours (all vertices of the sc need to have 3 neighbours)
    if(G[v].size() != 3) return;
    // check if v already in a live six cycle
    for(std::set<int> sc : live_six_cycles){
        if(sc.contains(v)) return;
    }

    // get unforced neighbours of v
    int w = -1; int u = -1;
    for(const auto& [e, o] : G[v]){
        if(forced_in_current[v].contains(e)) continue;
        if(w == -1) w = e;
        else u = e;
    }
    if(u == -1) return; // v has less than 2 unforced neighbours

    // check if w and u have 3 neighbours
    if(G[w].size() != 3 || G[u].size() != 3) return;

    // check every chain of unforced edges past w if it connects to u (only check that they dont go back)
    for(const auto& [x, x_o] : G[w]){
        if(x == v || forced_in_current[w].contains(x)) continue;
        for(const auto& [y, y_o] : G[x]){
            if(y == w || forced_in_current[x].contains(y)) continue;
            for(const auto& [z, z_o] : G[y]){
                if(z == x || forced_in_current[y].contains(z)) continue;
                // if u is an unforced neighbour of z, a six cycle is possible
                if(G[z].contains(u) && !forced_in_current[z].contains(u)){
                    std::set<int> sc = {v, w, u, x, y, z}; // possible six cycle
                    if(sc.size() != 6) continue; // enshures that no vertex appears twice in the cycle

                    // life six cycle found -> add to set an return
                    live_six_cycles.insert(sc);

                    std::cout << "live six cycle found: ";
                    for(int i : sc){
                        std::cout << i << " ";
                    }
                    std::cout << std::endl;

                    std::function<bool()> release_six_cycle = [sc]{
                        std::cout << "action: release live six cycle" << std::endl;
                        live_six_cycles.erase(sc);
                        return false;
                    };
                    actions.push_back(release_six_cycle);
                    return;
                }
            }
        }
    }
}

void erase_live_six_cycle(std::set<int> sc){
    std::cout << "erase life six cycle" << std::endl;
    live_six_cycles.erase(sc);

    std::function<bool()> unerase_sc = [sc]{
        live_six_cycles.insert(sc);
        return false;
    };
    actions.push_back(unerase_sc);
}
