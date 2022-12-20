#include <iostream>
#include <cstring>
#include "shortest_path.h"

int main(int argc, char **argv) {

    if(cmd_option_exists(argv, argv+argc, "--help"))
    {
        if (std::strcmp(argv[1],"--help") == 0) {
            std::cout << "Usage: short-path -s start-vertex -f input.txt\n"
                         "This program expects directed or undirected weighted graph with possible negative edges without loops\n"
                         "Prints all paths from starting point to all other points in graph\n"
                         "\n"
                         "Types of flags you can use\n"
                         "  --help                     shows this help\n"
                         "  -s                         starting vertex\n"
                         "  -f                         name of file with graph\n"
                         "  -a                         name of algorithm you want to use (D - dijkstra, B - bellman-ford), default is bellman-ford\n"
                         "\n"
                         "Example of how a graph file should look like:\n"
                         "S (vertex name) : { vertexes split by ',' } (vertex = path size) A=10,E=8\n";
        }
        return 0;
    }

    char * filename = get_cmd_option(argv, argv + argc, "-f");
    char * start_vertex = get_cmd_option(argv, argv + argc, "-s");
    char *algorithm;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        algorithm = get_cmd_option(argv, argv + argc, "-a");
    }


    if (filename)
    {
        std::ifstream file;
        file.open(filename);
        if(file) {
            if (start_vertex) {
                auto graph = prepare_file_for_graph(file);
                print_graph(graph);
                if (cmd_option_exists(argv, argv+argc, "-a")) {
                    if (std::strcmp(algorithm, "D") == 0) {
                        std::cout << "dijkstra";
                        dijkstra(graph);
                    } else if (std::strcmp(algorithm, "B") == 0) {
                        std::cout << "bellman-ford";
                    } else {
                        std::cerr << "we do not support this algorithm yet";
                        return -1;
                    }
                } else {
                    std::cout << "bellman-ford";
                }
            } else {
                std::cerr << "you need to provide starting vertex to compute path" << std::endl;
                return -1;
            }
        } else {
            std::cerr << "you need to provide filename of existing file" << std::endl;
            return -1;
        }
    } else {
        std::cerr << "you need to provide filename" << std::endl;
        return -1;
    }

    return 0;
}
