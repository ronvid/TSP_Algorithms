#include "CubicHam.cpp"

#include <iostream>

std::map<int, std::map<int, bool>> input;

void print_graph(std::map<int, std::map<int, bool>>* G);

int main(){

    input = {
        { 0 , { {1,true} , {2,true} , {3,true} } },
        { 1 , { {0,true} , {2,true} , {3,true} } },
        { 2 , { {0,true} , {1,true} , {3,true} } },
        { 3 , { {0,true} , {1,true} , {2,true} } }
    };
    G = {
        { 0 , { {1,true} , {2,true} , {3,true} } },
        { 1 , { {0,true} , {2,true} , {3,true} } },
        { 2 , { {0,true} , {1,true} , {3,true} } },
        { 3 , { {0,true} , {1,true} , {2,true} } }
    };

    ShortestHamiltonianCycle(&input);
    //remove(1,2);

    std::cout << " --- input --- " << std::endl;
    print_graph(&input);
    std::cout << " --- graph --- " << std::endl;
    print_graph(&G);
    std::cout << " --- forced_in_current --- " << std::endl;
    print_graph(&forced_in_current);
    std::cout << " --- forced_in_input --- " << std::endl;
    print_graph(&forced_in_input);

    return 0;
}

void print_graph(std::map<int, std::map<int, bool>>* G){
    for(const auto& [v, e] : *G){
        std::cout << v << ": ";
        for(const auto& [w, og] : e){
            std::cout << w << "." << og << " ";
        }
        std::cout << std::endl;
    }
}
