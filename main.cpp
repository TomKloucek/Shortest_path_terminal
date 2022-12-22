#include <iostream>
#include <cstring>
#include <chrono>
#include "shortest_path.h"

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

int execute(std::ifstream &file,bool write_result_to_file,int argc, char **argv,char *algorithm,const char * start_vertex, const char * result_filename, bool measure_time) {
    auto graph = prepare_file_for_graph(file);
    std::vector<std::pair<std::basic_string<char>, int>> paths;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        if (std::strcmp(algorithm, "D") == 0) {
            if (computing_path_possible(graph)) {
                if (!check_if_starting_vertex_exists(graph,start_vertex)) {
                    std::cerr << "provided vertex does not exists in graph" << std::endl;
                    return -1;
                }
                std::chrono::milliseconds measured_time;
                if (measure_time) {
                    auto start = std::chrono::high_resolution_clock::now();
                    paths = dijkstra(graph, start_vertex);
                    auto end = std::chrono::high_resolution_clock::now();
                    measured_time = to_ms(end - start);
                }
                else {
                    paths = dijkstra(graph, start_vertex);
                }
                if (paths.empty()) {
                    return -1;
                } else {
                    if (write_result_to_file) {
                        write_results_to_file(paths, start_vertex, result_filename, "Dijkstra",false,measure_time,measured_time.count());
                    }
                    print_path(paths, start_vertex);
                    return 1;
                }
            } else {
                std::cerr << "provided graph does not meet one of the following rules: all vertex names can be used only once" << std::endl;
                return -1;
            }
        } else if (std::strcmp(algorithm, "B") == 0) {
            if (computing_path_possible(graph)) {
                if (!check_if_starting_vertex_exists(graph,start_vertex)) {
                    std::cerr << "provided vertex does not exists in graph" << std::endl;
                    return -1;
                }
                paths = bellman_ford(graph, start_vertex);
                if (paths.empty()) {
                    return -1;
                } else {
                    print_path(paths, start_vertex);
                    return 0;
                }
            } else {
                std::cerr << "provided graph does not meet one of the following rules: all vertex names can be used only once" << std::endl;
                return -1;
            }
        } else {
            std::cerr << "we do not support this algorithm yet" << std::endl;
            return -1;
        }
    } else {
        std::cout << "bellman-ford";
        return 0;
    }
}

int execute_for_all_vertexes(std::ifstream &file,bool write_result_to_file,int argc, char **argv,char *algorithm,const char * start_vertex, const char * result_filename, bool measure_time) {
    auto graph = prepare_file_for_graph(file);
    std::vector<std::vector<std::pair<std::basic_string<char>, int>>> path_set;
    std::vector<std::pair<std::basic_string<char>, int>> paths;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        if (std::strcmp(algorithm, "D") == 0) {
            if (computing_path_possible(graph)) {
                std::chrono::milliseconds measured_time;
                if (measure_time) {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (auto const& vertex : graph) {
                        path_set.emplace_back(dijkstra(graph, vertex.first));
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    measured_time = to_ms(end - start);
                }
                else {
                    for (auto const& vertex : graph) {
                        path_set.emplace_back(dijkstra(graph, vertex.first));
                    }                }
                if (path_set.empty()) {
                    return -1;
                } else {
                    int counter = 0;
                    prepare_file_to_appending(result_filename, "Dijkstra", measure_time, measured_time.count());
                    for (auto const& path : path_set) {
                        print_path(path, graph[counter].first);
                        write_results_to_file(path, graph[counter].first, result_filename, "Dijkstra",true,measure_time,measured_time.count());
                        std::cout << "\n";
                        counter++;
                    }
                    return 1;
                }
            } else {
                std::cerr << "provided graph does not meet one of the following rules: { all vertex names can be used only once, edge must not end in not existing vertex }" << std::endl;
                return -1;
            }
        } else if (std::strcmp(algorithm, "B") == 0) {
            if (computing_path_possible(graph)) {
                if (!check_if_starting_vertex_exists(graph,start_vertex)) {
                    std::cerr << "provided vertex does not exists in graph" << std::endl;
                    return -1;
                }
                paths = bellman_ford(graph, start_vertex);
                if (paths.empty()) {
                    return -1;
                } else {
                    print_path(paths, start_vertex);
                    return 0;
                }
            } else {
                std::cerr << "provided graph does not meet one of the following rules: all vertex names can be used only once" << std::endl;
                return -1;
            }
        } else {
            std::cerr << "we do not support this algorithm yet" << std::endl;
            return -1;
        }
    } else {
        std::cout << "bellman-ford";
        return 0;
    }
}

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

    char * filename = get_cmd_option(argv, argv + argc, "-f");
    char * start_vertex = get_cmd_option(argv, argv + argc, "-s");
    char *algorithm;
    char * result_filename;
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
