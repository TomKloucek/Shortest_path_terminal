#include <iostream>
#include <cstring>
#include <chrono>
#include "executes.h"

int main(int argc, char **argv) {
    if (!parameters_are_right(argv,argc)) {
        std::cerr << "parameters of the program contains unsupported flags" << std::endl;
        return -1;
    }
    if(cmd_option_exists(argv, argv+argc, "--help"))
    {
        if (std::strcmp(argv[1],"--help") == 0) {
            std::cout << "Usage: short-path -s start-vertex -f input.txt\n"
                         "This program expects directed or undirected weighted graph with possible negative edges without loops\n"
                         "Prints all paths from starting point to all other points in graph\n"
                         "\n"
                         "Types of flags you can use\n"
                         "  --help                     shows this help\n"
                         "  -s                         starting vertex (if not specified, paths from all vertex will be computed)\n"
                         "  -f                         name of file with graph\n"
                         "  -w                         name of file where you want to save results\n"
                         "  -t                         time of calculation will be measured\n"
                         "  -a                         name of algorithm you want to use (D - dijkstra, B - bellman-ford), default is bellman-ford\n"
                         " Algorithms usage:\n"
                         " Dijkstra: can be used for computing path from single source on both directed and undirected non negative weighted graphs\n"
                         " Bellman-Ford: can be used for computing path from single source on directed negative weighted graphs and non negative undirected weighted graphs\n"
                         "\n"
                         "Example of how a graph file should look like:\n"
                         "S (vertex name) : { vertexes split by ',' } (vertex = path size) A=10,E=8\n";
        }
        return 0;
    }

    char const * filename = get_cmd_option(argv, argv + argc, "-f");
    char const * start_vertex = get_cmd_option(argv, argv + argc, "-s");
    char *algorithm;
    char const * result_filename;
    bool write_result_to_file = false;
    bool measure_time = false;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        algorithm = get_cmd_option(argv, argv + argc, "-a");
    }
    if (cmd_option_exists(argv, argv+argc, "-w")) {
        write_result_to_file = true;
        result_filename = get_cmd_option(argv, argv + argc, "-w");
    }
    if (cmd_option_exists(argv, argv+argc, "-t")) {
        measure_time = true;
    }

    if (filename)
    {
        std::ifstream file;
        file.open(filename);
        if(file) {
            if (start_vertex) {
                return execute(file, write_result_to_file,argc,argv,algorithm,start_vertex,result_filename, measure_time);
            } else {
                return execute_for_all_vertexes(file, write_result_to_file,argc,argv,algorithm,start_vertex,result_filename, measure_time);
            }
        } else {
            std::cerr << "you need to provide filename of existing file" << std::endl;
            return -1;
        }
    } else {
        std::cerr << "you need to provide filename" << std::endl;
        return -1;
    }
}
