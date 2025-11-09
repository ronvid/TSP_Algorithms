#include "Eppstein.hpp"
#include "Bruteforce.hpp"
#include "Schuster.hpp"
#include "Schuster_New_Heuristic.hpp"

#include "Generator.cpp"

#include <ctime>
#include <chrono>

// will compare eppstein and bruteforce on a graph of the given size; if always_show is true, additional information will always be shown
// returns false if the returned costs are not the same and will print additional information
bool compare_algorithms(int size, bool epp, bool brute, bool schuster, bool schuster_nh, int type, bool always_show=false){
    // success flag
    bool success = true;

    // generate seed for
    int seed = std::rand();
    write_seed_to_file(seed, size); // write seed to file

    // generate graph of given size
    std::unordered_map<int, std::unordered_map<int, int>>* generated = generate_random_graph(size, seed, type);
    // init values
    std::unordered_map<int, std::unordered_map<int, bool>> e_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> b_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> s_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> snh_edges;
    int e_cost = -1; bool e_succ = true;
    int b_cost = -1; bool b_succ = true;
    int s_cost = -1; bool s_succ = true;
    int snh_cost = -1; bool snh_succ = true;

    //to_dot(generated, &e_edges, "g_test");

    std::cout << "Size: " << generated->size() << " Seed: " << seed << std::endl;

    // eppstein
    if(epp){
        const auto start{std::chrono::steady_clock::now()};
        e_succ = Eppstein::ShortestHamiltonianCycle(generated, &e_edges, &e_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Eppstein: " << e_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open (".e_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // bruteforce
    if(brute){
        const auto start{std::chrono::steady_clock::now()};
        b_succ = Bruteforce::BruteforceHamiltonianCycle(generated, &b_edges, &b_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Bruteforce: " << b_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open (".b_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // Schuster
    if(schuster){
        const auto start{std::chrono::steady_clock::now()};
        s_succ = Schuster::ShortestHamiltonianCycle(generated, &s_edges, &s_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Schuster: " << s_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open (".s_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // Schuster with new heuristic
    if(schuster_nh){
        const auto start{std::chrono::steady_clock::now()};
        snh_succ = Schuster_New_Heuristic::ShortestHamiltonianCycle(generated, &snh_edges, &snh_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Schuster (new heuristic): " << snh_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open (".snh_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // check if all used algorithms came to the same conclusion
    if((!epp && !brute) || (!epp && !schuster) || (!brute && !schuster)){
        success = true;
    }
    else if(!epp){
        success = (b_succ == s_succ) && (b_cost == s_cost);
    }
    else if(!brute){
        success = (e_succ == s_succ) && (e_cost == s_cost);
    }
    else if(!schuster){
        success = (e_succ == b_succ) && (e_cost == b_cost);
    }
    else{
        success = (e_succ == b_succ) && (b_succ == s_succ) && (e_cost == b_cost) && (b_cost == s_cost);
    }

    // TODO x_succ could be removed: algorithms can fail if graph has no cycle
    if(success){
        std::cout << " Success!" << std::endl;
    }
    else{
        std::cout << " Error!" << std::endl;
        if(!e_succ){
            std::cout << "Eppstein failed" << std::endl;
        }
        if(!b_succ){
            std::cout << "Bruteforce failed" << std::endl;
        }
        if(!s_succ){
            std::cout << "Schuster failed" << std::endl;
        }
        if(!snh_succ){
            std::cout << "Schuster (new heuristic) failed" << std::endl;
        }
        if(e_cost != b_cost || e_cost != s_cost || b_cost != s_cost){
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
        std::cout << "Schuster:" << std::endl;
        print_graph(&s_edges);

        // create dot files with the graphs
        to_dot(generated, &e_edges, "eppstein");
        to_dot(generated, &b_edges, "bruteforce");
        to_dot(generated, &s_edges, "schuster");
        std::cout << "Graphs have been written to .gv files" << std::endl;
    }

    // dealloc generated graph
    delete generated;

    return success;

}

int main(){


    // clear time measurement files
    std::ofstream file;
    file.open(".e_runtime.txt");
    file << "";
    file.close();
    file.open(".b_runtime.txt");
    file << "";
    file.close();
    file.open(".s_runtime.txt");
    file << "";
    file.close();
    file.open(".snh_runtime.txt");
    file << "";
    file.close();

    // clear time measurement files (TODO remove)
    file.open(".six_cycles.txt");
    file << "";
    file.close();

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

    int type;
    std::cout << "Enter type of graph to be generated[0=random cycle, 1=random regular, 2=high hamiltonian]" << std::endl;
    std::cin >> type;

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

    bool run_schuster_nh = true;
    std::cout << "Test Schuser (new heuristic)? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_schuster_nh = false; }

    // write stats to file
    file.open(".stats.txt");
    file << min_size <<"\n" << max_size << "\n" << repetitions << "\n";
    file.close();

    for(int i = min_size; i <= max_size; i++){
        if(type == HIGH_HAMILTONIAN && i%6 != 0) continue; // high hamiltonian only works with graphs size multiple of 6
        if(type == RANDOM_REGULAR && i%2 != 0) continue; // random regular only  works with multiple of 2
        bool stop = false;
        for(int j = 0; j < repetitions; j++){
            if(!compare_algorithms(i, run_eppstein, run_bruteforce, run_schuster, run_schuster_nh, type)){
                stop = true;
                break;
            }
        }
        if(stop) break;
    }

    return 0;
}
