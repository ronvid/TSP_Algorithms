#include "Eppstein.hpp"
#include "Bruteforce.hpp"
#include "Schuster.hpp"
#include "Schuster_New_Heuristic.hpp"
#include "Schuster_Simple_Choice.hpp"
#include "Schuster_High_Choice.hpp"

#include "Generator.cpp"

#include <ctime>
#include <chrono>

// will compare eppstein and bruteforce on a graph of the given size; if always_show is true, additional information will always be shown
// returns false if the returned costs are not the same and will print additional information
bool compare_algorithms(int size, bool epp, bool brute, bool schuster, bool nh_fixed, bool nh_dynamic, bool nh_simple, bool nh_high, int type, bool always_show=false){

    // generate seed for
    int seed = std::rand();
    write_seed_to_file(seed, size); // write seed to file

    // generate graph of given size
    std::unordered_map<int, std::unordered_map<int, int>>* generated = generate_random_graph(size, seed, type);
    // init values
    std::unordered_map<int, std::unordered_map<int, bool>> e_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> b_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> s_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> fixed_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> dynamic_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> simple_edges;
    std::unordered_map<int, std::unordered_map<int, bool>> high_edges;
    int e_cost = -1;
    int b_cost = -1;
    int s_cost = -1;
    int fixed_cost = -1;
    int dynamic_cost = -1;
    int simple_cost = -1;
    int high_cost = -1;

    //to_dot(generated, &e_edges, "g_test");

    std::cout << "Size: " << generated->size() << " Seed: " << seed << std::endl;

    // eppstein
    if(epp){
        const auto start{std::chrono::steady_clock::now()};
        Eppstein::ShortestHamiltonianCycle(generated, &e_edges, &e_cost);
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
    if(brute && size < 41){
        const auto start{std::chrono::steady_clock::now()};
        Bruteforce::BruteforceHamiltonianCycle(generated, &b_edges, &b_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Bruteforce: " << b_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open(".b_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // Schuster
    if(schuster){
        const auto start{std::chrono::steady_clock::now()};
        Schuster::ShortestHamiltonianCycle(generated, &s_edges, &s_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Schuster: " << s_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open(".s_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    if(nh_fixed){
        // test fixed values
        for(long unsigned int h : {10, 20, 30}){
            const auto start{std::chrono::steady_clock::now()};
            Schuster_New_Heuristic::ShortestHamiltonianCycle(generated, &fixed_edges, &fixed_cost, h);
            const auto finish{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{finish - start};
            std::cout << "Schuster (h = " << h << "): "<< fixed_cost << " in " << elapsed_seconds << "." << std::endl;

            // open file and write runtime
            std::ofstream file;
            std::string path = ".fixed_runtime_" + std::to_string(h) + ".txt";
            file.open(path, std::ios::out | std::ios::app);
            file << elapsed_seconds.count() << "\n";
            file.close();
        }
    }
    if(nh_dynamic){
        // test dynamic values
        for(float h : {0.15, 0.30, 0.45}){
            const auto start{std::chrono::steady_clock::now()};
            Schuster_New_Heuristic::ShortestHamiltonianCycle(generated, &dynamic_edges, &dynamic_cost, 0, h);
            const auto finish{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{finish - start};
            std::cout << "Schuster (h = " << h << "%): "<< dynamic_cost << " in " << elapsed_seconds << "." << std::endl;

            // open file and write runtime
            std::ofstream file;
            std::string path = ".dynamic_runtime_" + std::to_string(h) + ".txt";
            file.open(path, std::ios::out | std::ios::app);
            file << elapsed_seconds.count() << "\n";
            file.close();
        }
    }

    if(nh_simple){
        const auto start{std::chrono::steady_clock::now()};
        Schuster_Simple_Choice::ShortestHamiltonianCycle(generated, &simple_edges, &simple_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Schuster simple choice: " << simple_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open(".simple_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    if(nh_high){
        const auto start{std::chrono::steady_clock::now()};
        Schuster_High_Choice::ShortestHamiltonianCycle(generated, &high_edges, &high_cost);
        const auto finish{std::chrono::steady_clock::now()};
        const std::chrono::duration<double> elapsed_seconds{finish - start};
        std::cout << "Schuster high choice: " << high_cost << " in " << elapsed_seconds << "." << std::endl;

        // open file and write runtime
        std::ofstream file;
        file.open(".high_runtime.txt", std::ios::out | std::ios::app);
        file << elapsed_seconds.count() << "\n";
        file.close();
    }

    // dealloc generated graph
    delete generated;

    return true;

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
    file.open(".snh_runtime_10.txt");
    file << "";
    file.close();
    file.open(".snh_runtime_20.txt");
    file << "";
    file.close();
    file.open(".snh_runtime_30.txt");
    file << "";
    file.close();

    file.open(".snh_runtime_0.150000.txt");
    file << "";
    file.close();

    file.open(".snh_runtime_0.300000.txt");
    file << "";
    file.close();

    file.open(".snh_runtime_0.450000.txt");
    file << "";
    file.close();

    file.open(".simple_runtime.txt");
    file << "";
    file.close();

    file.open(".dynamic_runtime.txt");
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

    bool run_fixed= true;
    std::cout << "Test Schuser (fixed heuristic)? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_fixed = false; }

    bool run_dynamic = true;
    std::cout << "Test Schuser (dynamic heuristic)? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_dynamic = false; }

    bool run_simple = true;
    std::cout << "Test Schuser (simple choice for 6-cycle)? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_simple = false; }

    bool run_high = true;
    std::cout << "Test Schuser (highest choice for 6-cycle)? (y/n)" << std::endl;
    std::cin >> in;
    if(in == 'n'){ run_high= false; }


    // write stats to file
    file.open(".stats.txt");
    file << min_size <<"\n" << max_size << "\n" << repetitions << "\n";
    file.close();

    for(int i = min_size; i <= max_size; i++){
        if(type == HIGH_HAMILTONIAN && i%6 != 0) continue; // high hamiltonian only works with graphs size multiple of 6
        if(type == RANDOM_REGULAR && i%2 != 0) continue; // random regular only  works with multiple of 2
        bool stop = false;
        for(int j = 0; j < repetitions; j++){
            if(!compare_algorithms(i, run_eppstein, run_bruteforce, run_schuster, run_fixed, run_dynamic, run_simple, run_high,  type)){
                stop = true;
                break;
            }
        }
        if(stop) break;
    }

    return 0;
}
