#include "CubicHam.cpp"

#include <iostream>

void print_graph(std::map<int, std::map<int, bool>>* G);
void print_graph(std::map<int, std::map<int, int>>* W);

int main(){

    std::map<int, std::map<int, bool>> input;
    std::map<int, std::map<int, int>> input_weights;
    std::map<int, std::map<int, bool>> forced_edges;

    int cycle_cost = 0;

    input = {
        { 0 , { {1,true} , {2,true} , {3,true} } },
        { 1 , { {0,true} , {2,true} , {3,true} } },
        { 2 , { {0,true} , {1,true} , {3,true} } },
        { 3 , { {0,true} , {1,true} , {2,true} } }
    };

    input_weights = {
        { 0 , { {1, 1} , {2, 2} , {3, 3} } },
        { 1 , { {0, 1} , {2, 4} , {3, 6} } },
        { 2 , { {0, 2} , {1, 4} , {3, 5} } },
        { 3 , { {0, 3} , {1, 6} , {2, 5} } }
    };

    /*input = {
    { 0 , { {1,true} , {9,true} , {10,true} } },
    { 1 , { {2,true} , {3,true} , {0,true} } },
    { 2 , { {1,true} , {3,true} , {10,true} } },
    { 3 , { {1,true} , {2,true} , {4,true} } },
    { 4 , { {3,true} , {5,true} , {6,true} } },
    { 5 , { {4,true} , {6,true} } },
    { 6 , { {4,true} , {5,true} , {7,true} } },
    { 7 , { {6,true} , {8,true} , {9,true} } },
    { 8 , { {7,true} , {9,true} } },
    { 9 , { {7,true} , {8,true} , {0,true} } },
    { 10, { {0,true} , {2,true} } }
    };
    */
    /*G = {
        { 0 , { {1,true} , {2,true} , {3,true} } },
        { 1 , { {0,true} , {2,true} , {3,true} } },
        { 2 , { {0,true} , {1,true} , {3,true} } },
        { 3 , { {0,true} , {1,true} , {2,true} } }
    };
    */

    ShortestHamiltonianCycle(&input, &input_weights, &forced_edges, &cycle_cost);
    //remove(1,2);

    std::cout << " --- input --- " << std::endl;
    print_graph(&input);
    std::cout << " --- weights --- " << std::endl;
    print_graph(&W);
    std::cout << " --- graph --- " << std::endl;
    print_graph(&G);
    std::cout << " --- forced_in_current --- " << std::endl;
    print_graph(&forced_in_current);
    std::cout << " --- forced_in_input --- " << std::endl;
    print_graph(&forced_in_input);
    std::cout << " --- found cycle --- " << cycle_cost << std::endl;
    print_graph(&forced_edges);

    std::cout << " --- triangles: " << triangles.size() << " --- " << std::endl;
    for(auto i : triangles){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
