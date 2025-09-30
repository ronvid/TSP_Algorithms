#include "Eppstein.hpp"
#include "Bruteforce.hpp"
#include "Schuster.hpp"

#include "Generator.cpp"

#include <ctime>

// will compare eppstein and bruteforce on a graph of the given size; if always_show is true, additional information will always be shown
// returns false if the returned costs are not the same and will print additional information
bool compare_algorithms(int size, bool epp, bool brute, bool schuster, bool always_show=false){
    // success flag
    bool success = true;

    // generate seed for
    int seed = std::rand();

    // generate graph of given size
    std::unordered_map<int, std::unordered_map<int, int>>* generated = generate_random_graph(size, seed);
    // init values
    std::unordered_map<int, std::unordered_map<int, bool>> e_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> b_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> s_edges;
    int e_cost = 0; bool e_succ;
    int b_cost = 0; bool b_succ;
    int s_cost = 0; bool s_succ;

    std::cout << "Size: " << generated->size() << " Seed: " << seed << std::endl;

    // eppstein
    if(epp){
        e_succ = Eppstein::ShortestHamiltonianCycle(generated, &e_edges, &e_cost);
        std::cout << "Eppstein: " << e_cost << std::endl;
    }

    // bruteforce
    if(brute){
        b_succ = Bruteforce::BruteforceHamiltonianCycle(generated, &b_edges, &b_cost);
        std::cout << "Bruteforce: " << b_cost << std::endl;
    }

    // Schuster
    if(schuster){
        s_succ = Schuster::ShortestHamiltonianCycle(generated, &s_edges, &s_cost);
        std::cout << "Schuster: " << s_cost << std::endl;
    }

    // TODO x_succ could be removed: algorithms can fail if graph has no cycle
    if((e_succ && s_succ && e_cost == s_cost) || !epp || !schuster){
        std::cout << " Success!" << std::endl;
    }
    else{
        std::cout << " Error!" << std::endl;
        success = false;
        write_seed_to_file(seed, size);
        std::cout << "Wrote seed to file!" << std::endl;
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
    if(!success || always_show){
        std::cout << "Details: \n Graph:" << std::endl;
        print_graph(generated);
        std::cout << "Eppstein:" << std::endl;
        print_graph(&e_edges);
        std::cout << "Bruteforce:" << std::endl;
        print_graph(&b_edges);

        // create dot files with the graphs
        to_dot(generated, &e_edges, "eppstein");
        to_dot(generated, &b_edges, "bruteforce");
        std::cout << "Graphs have been written to .gv files" << std::endl;
    }

    // dealloc generated graph
    delete generated;

    return success;

}

int main(){

    // init rng
    std::srand(std::time({}));

    // get inputs
    int min_size = 3;
    std::cout << "Enter min size of tested graphs (greater than 2) [" << min_size << "]" << std::endl;
    std::cin >> min_size;

    int max_size = 30;
    std::cout << "Enter max size of tested graphs [" << max_size << "]" << std::endl;
    std::cin >> max_size;

    int repetitions = 3;
    std::cout << "Enter number of graphs of one size to be tested[" << repetitions << "]" << std::endl;
    std::cin >> repetitions;

    char in;
    bool run_eppstein = true;
    std::cout << "Test Eppstein? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_eppstein = false; }

    bool run_bruteforce = true;
    std::cout << "Test Bruteforce? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_bruteforce = false; }

    bool run_schuster = true;
    std::cout << "Test Schuser? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_schuster = false; }

    for(int i = min_size; i < max_size; i++){
        bool stop = false;
        for(int j = 0; j < repetitions; j++){
            if(!compare_algorithms(i, run_eppstein, run_bruteforce, run_schuster)){
                stop = true;
                break;
            }
        }
        if(stop) break;
    }

    return 0;
}
