#include "Eppstein.hpp"
#include "Bruteforce.hpp"

#include "Generator.cpp"

#include <ctime>

// will compare eppstein and bruteforce on a graph of the given size; if always_show is true, additional information will always be shown
// returns false if the returned costs are not the same and will print additional information
bool compare_algorithms(int size, bool always_show=false){
    // success flag
    bool success = true;

    // generate graph of given size
    int seed = std::time({});
    std::unordered_map<int, std::unordered_map<int, int>>* generated = generate_random_graph(size, seed);
    // init values
    std::unordered_map<int, std::unordered_map<int, bool>> e_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> b_edges;
    int e_cost = 0; bool e_succ;
    int b_cost = 0; bool b_succ;

    std::cout << "Size: " << size;

    // run algorithms
    e_succ = Eppstein::ShortestHamiltonianCycle(generated, &e_edges, &e_cost);
    std::cout << " Eppstein: " << e_cost;

    b_succ = Bruteforce::BruteforceHamiltonianCycle(generated, &b_edges, &b_cost);
    std::cout << " Bruteforce: " << b_cost;

    // TODO x_succ could be removed: algorithms can fail if graph has no cycle
    if(e_succ && b_succ && e_cost == b_cost){
        std::cout << " Success!" << std::endl;
    }
    else{
        std::cout << " Error!" << std::endl;
        success = false;
        if(!e_succ){
            std::cout << "Eppstein failed" << std::endl;
        }
        if(!b_succ){
            std::cout << "Bruteforce failed" << std::endl;
        }
        if(e_cost != b_cost){
            std::cout << "Costs in algorithms are different" << std::endl;
        }
    }

    // additional information will be shown if unsuccessful or propted to always show
    if(success || always_show){
        std::cout << "Details: \n Graph:" << std::endl;
        print_graph(generated);
        std::cout << "Eppstein:" << std::endl;
        print_graph(&e_edges);
        std::cout << "Bruteforce:" << std::endl;
        print_graph(&b_edges);
    }

    // dealloc generated graph
    delete generated;

    return success;

}

int main(){

    compare_algorithms(10, true);

    return 0;
}
