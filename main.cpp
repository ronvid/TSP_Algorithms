#include "CubicHam.cpp"

#include <iostream>

void print_graph(std::map<int, std::map<int, bool>>* G);
void print_graph(std::map<int, std::map<int, int>>* W);

int main(){

    std::map<int, std::map<int, bool>> input;
    std::map<int, std::map<int, int>> input_weights;
    std::map<int, std::map<int, bool>> forced_edges;

    int cycle_cost = 0;
/*
    add_edge(&input, &input_weights, 0, 1, 1);
    add_edge(&input, &input_weights, 0, 2, 2);
    add_edge(&input, &input_weights, 0, 3, 3);
    add_edge(&input, &input_weights, 1, 2, 4);
    add_edge(&input, &input_weights, 2, 3, 5);
    add_edge(&input, &input_weights, 3, 1, 6);*/

    //basic graph
//     add_edge(&input, &input_weights, 0, 1, 1);
//     add_edge(&input, &input_weights, 1, 2, 2);
//     add_edge(&input, &input_weights, 2, 3, 3);
//     add_edge(&input, &input_weights, 3, 4, 4);
//     add_edge(&input, &input_weights, 4, 5, 5);
//     add_edge(&input, &input_weights, 5, 6, 6);
//     add_edge(&input, &input_weights, 6, 7, 7);
//     add_edge(&input, &input_weights, 7, 8, 8);
//     add_edge(&input, &input_weights, 8, 9, 9);
//     add_edge(&input, &input_weights, 9, 0, 10);
//     add_edge(&input, &input_weights, 1, 3, 11);
//     add_edge(&input, &input_weights, 4, 6, 12);
//     add_edge(&input, &input_weights, 7, 9, 13);
//     add_edge(&input, &input_weights, 0, 10, 14);
//     add_edge(&input, &input_weights, 10, 2, 15);
//
/*
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
*/

    // double fc
/*    add_edge(&input, &input_weights, 1 , 2 , 1 );
    add_edge(&input, &input_weights, 1 , 3 , 2 );
    add_edge(&input, &input_weights, 1 , 7 , 3 );
    add_edge(&input, &input_weights, 2 , 4 , 4 );
    add_edge(&input, &input_weights, 2 , 5 , 5 );
    add_edge(&input, &input_weights, 3 , 4 , 6 );
    add_edge(&input, &input_weights, 3 , 6 , 7 );
    add_edge(&input, &input_weights, 4 , 9 , 8 );
    add_edge(&input, &input_weights, 5 , 8 , 9 );
    add_edge(&input, &input_weights, 6 , 10, 10);
    add_edge(&input, &input_weights, 7 , 8 , 11);
    add_edge(&input, &input_weights, 7 , 10, 12);
    add_edge(&input, &input_weights, 8 , 9 , 13);
    add_edge(&input, &input_weights, 9 , 10, 14);
*/
/*
    // test mst
    add_edge(&input, &input_weights, 1 , 2 , 1 );
    add_edge(&input, &input_weights, 1 , 3 , 2 );
    add_edge(&input, &input_weights, 1 , 13, 3 );
    add_edge(&input, &input_weights, 2 , 4 , 4 );
    add_edge(&input, &input_weights, 2 , 14, 5 );
    add_edge(&input, &input_weights, 3 , 4 , 6 );
    add_edge(&input, &input_weights, 3 , 16, 7 );
    add_edge(&input, &input_weights, 4 , 15, 8 );
    add_edge(&input, &input_weights, 5 , 6 , 9 );
    add_edge(&input, &input_weights, 5 , 7 , 10);
    add_edge(&input, &input_weights, 6 , 8 , 11);
    add_edge(&input, &input_weights, 6 , 17, 12);
    add_edge(&input, &input_weights, 7 , 8 , 13);
    add_edge(&input, &input_weights, 8 , 18, 14);
    add_edge(&input, &input_weights, 9 , 10, 15);
    add_edge(&input, &input_weights, 9 , 11, 16);
    add_edge(&input, &input_weights, 10, 12, 18);
    add_edge(&input, &input_weights, 11, 12, 18);
    add_edge(&input, &input_weights, 13, 5 , 0 );
    add_edge(&input, &input_weights, 14, 7 , 0 );
    add_edge(&input, &input_weights, 15, 11, 0 );
    add_edge(&input, &input_weights, 16, 12, 0 );
    add_edge(&input, &input_weights, 17, 10, 0 );
    add_edge(&input, &input_weights, 18, 9 , 0 );
*/
    // complex fc mst test
    add_edge(&input, &input_weights, 0 , 1 , 1 );
    add_edge(&input, &input_weights, 0 , 2 , 3 );
    add_edge(&input, &input_weights, 1 , 3 , 4 );
    add_edge(&input, &input_weights, 2 , 3 , 2 );
    add_edge(&input, &input_weights, 4 , 5 , 1 );
    add_edge(&input, &input_weights, 4 , 6 , 1 );
    add_edge(&input, &input_weights, 5 , 7 , 2 );
    add_edge(&input, &input_weights, 6 , 7 , 1 );
    add_edge(&input, &input_weights, 8 , 9 , 2 );
    add_edge(&input, &input_weights, 8 , 10, 3 );
    add_edge(&input, &input_weights, 9 , 11, 1 );
    add_edge(&input, &input_weights, 10, 11, 3 );
    add_edge(&input, &input_weights, 12, 13, 3 );
    add_edge(&input, &input_weights, 12, 14, 4 );
    add_edge(&input, &input_weights, 13, 15, 1 );
    add_edge(&input, &input_weights, 14, 15, 3 );
    add_edge(&input, &input_weights, 16, 17, 5 );
    add_edge(&input, &input_weights, 16, 18, 6 );
    add_edge(&input, &input_weights, 17, 19, 6 );
    add_edge(&input, &input_weights, 18, 19, 3 );
    add_edge(&input, &input_weights, 20, 21, 1 );
    add_edge(&input, &input_weights, 20, 22, 3 );
    add_edge(&input, &input_weights, 21, 23, 4 );
    add_edge(&input, &input_weights, 22, 23, 1 );
    add_edge(&input, &input_weights, 24, 25, 1 );
    add_edge(&input, &input_weights, 24, 26, 4 );
    add_edge(&input, &input_weights, 25, 27, 3 );
    add_edge(&input, &input_weights, 26, 27, 2 );
    add_edge(&input, &input_weights, 28, 29, 4 );
    add_edge(&input, &input_weights, 28, 30, 8 );
    add_edge(&input, &input_weights, 29, 31, 6 );
    add_edge(&input, &input_weights, 30, 31, 3 );
    add_edge(&input, &input_weights, 32, 33, 5 );
    add_edge(&input, &input_weights, 32, 34, 2 );
    add_edge(&input, &input_weights, 33, 35, 3 );
    add_edge(&input, &input_weights, 34, 35, 5 );
        // forced
    add_edge(&input, &input_weights, 0 , 50, 1 ); add_edge(&input, &input_weights, 50, 27, 1 );
    add_edge(&input, &input_weights, 1 , 51, 1 ); add_edge(&input, &input_weights, 51, 8 , 1 );
    add_edge(&input, &input_weights, 2 , 52, 1 ); add_edge(&input, &input_weights, 52, 4 , 1 );
    add_edge(&input, &input_weights, 3 , 53, 1 ); add_edge(&input, &input_weights, 53, 5 , 1 );
    add_edge(&input, &input_weights, 6 , 54, 1 ); add_edge(&input, &input_weights, 54, 20, 1 );
    add_edge(&input, &input_weights, 7 , 55, 1 ); add_edge(&input, &input_weights, 55, 12, 1 );
    add_edge(&input, &input_weights, 9 , 56, 1 ); add_edge(&input, &input_weights, 56, 17, 1 );
    add_edge(&input, &input_weights, 10, 57, 1 ); add_edge(&input, &input_weights, 57, 13, 1 );
    add_edge(&input, &input_weights, 11, 58, 1 ); add_edge(&input, &input_weights, 58, 16, 1 );
    add_edge(&input, &input_weights, 14, 59, 1 ); add_edge(&input, &input_weights, 59, 21, 1 );
    add_edge(&input, &input_weights, 15, 60, 1 ); add_edge(&input, &input_weights, 60, 28, 1 );
    add_edge(&input, &input_weights, 18, 61, 1 ); add_edge(&input, &input_weights, 61, 24, 1 );
    add_edge(&input, &input_weights, 19, 62, 1 ); add_edge(&input, &input_weights, 62, 25, 1 );
    add_edge(&input, &input_weights, 22, 63, 1 ); add_edge(&input, &input_weights, 63, 34, 1 );
    add_edge(&input, &input_weights, 23, 64, 1 ); add_edge(&input, &input_weights, 64, 32, 1 );
    add_edge(&input, &input_weights, 26, 65, 1 ); add_edge(&input, &input_weights, 65, 29, 1 );
    add_edge(&input, &input_weights, 33, 66, 1 ); add_edge(&input, &input_weights, 66, 30, 1 );
    add_edge(&input, &input_weights, 35, 67, 1 ); add_edge(&input, &input_weights, 67, 31, 1 );


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
    std::cout << " --- four cycles: " << four_cycles.size() << " --- " << std::endl;
    for(auto i : four_cycles){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
