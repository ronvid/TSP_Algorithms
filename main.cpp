#include "CubicHam.cpp"

#include <iostream>

void print_graph(std::map<int, std::map<int, bool>>* G);
void print_graph(std::map<int, std::map<int, int>>* W);

int main(){

    std::map<int, std::map<int, bool>> input;
    std::map<int, std::map<int, int>> input_weights;
    std::map<int, std::map<int, bool>> forced_edges;

    int cycle_cost = 0;

    /*input = {
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
    };*/

    // baseic graph
    // add_edge(&input, &input_weights, 0, 1, 1);
    // add_edge(&input, &input_weights, 1, 2, 2);
    // add_edge(&input, &input_weights, 2, 3, 3);
    // add_edge(&input, &input_weights, 3, 4, 4);
    // add_edge(&input, &input_weights, 4, 5, 5);
    // add_edge(&input, &input_weights, 5, 6, 6);
    // add_edge(&input, &input_weights, 6, 7, 7);
    // add_edge(&input, &input_weights, 7, 8, 8);
    // add_edge(&input, &input_weights, 8, 9, 9);
    // add_edge(&input, &input_weights, 9, 0, 10);
    // add_edge(&input, &input_weights, 1, 3, 11);
    // add_edge(&input, &input_weights, 4, 6, 12);
    // add_edge(&input, &input_weights, 7, 9, 13);
    // add_edge(&input, &input_weights, 0, 10, 14);
    // add_edge(&input, &input_weights, 10, 2, 15);

    add_edge(&input, &input_weights, 1 , 2 , 1 );
    add_edge(&input, &input_weights, 1 , 3 , 2 );
    add_edge(&input, &input_weights, 2 , 3 , 3 );
    add_edge(&input, &input_weights, 2 , 4 , 4 );
    add_edge(&input, &input_weights, 3 , 4 , 5 );
    add_edge(&input, &input_weights, 4 , 5 , 6 );
    add_edge(&input, &input_weights, 5 , 6 , 7 );
    add_edge(&input, &input_weights, 5 , 7 , 8 );
    add_edge(&input, &input_weights, 6 , 7 , 9 );
    add_edge(&input, &input_weights, 6 , 8 , 10);
    add_edge(&input, &input_weights, 7 , 8 , 11);
    add_edge(&input, &input_weights, 8 , 9 , 12);
    add_edge(&input, &input_weights, 9 , 10, 13);
    add_edge(&input, &input_weights, 9 , 11, 14);
    add_edge(&input, &input_weights, 10, 11, 15);
    add_edge(&input, &input_weights, 10, 12, 16);
    add_edge(&input, &input_weights, 11, 12, 17);
    add_edge(&input, &input_weights, 12, 1 , 18);

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
