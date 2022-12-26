#include "shortest_path.h"

template <typename TimePoint>
std::chrono::microseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::microseconds>(tp);
}

int execute(std::ifstream &file,bool write_result_to_file,int argc, char **argv,char *algorithm,const char * start_vertex, const char * result_filename, bool measure_time) {
    auto graph = prepare_file_for_graph(file);
    if (graph.empty()) {
        return -1;
    }
    std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>> paths;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        if (std::strcmp(algorithm, "D") == 0) {
            if (computing_path_possible(graph)) {
                if (!check_if_starting_vertex_exists(graph,start_vertex)) {
                    std::cerr << "provided vertex does not exists in graph" << std::endl;
                    return -1;
                }
                std::chrono::microseconds measured_time;
                if (measure_time) {
                    auto start = std::chrono::high_resolution_clock::now();
                    paths = dijkstra(graph, start_vertex);
                    auto end = std::chrono::high_resolution_clock::now();
                    measured_time = to_ms(end - start);
                }
                else {
                    paths = dijkstra(graph, start_vertex);
                }
                if (paths.first.empty()) {
                    return -1;
                } else {
                    if (write_result_to_file) {
                        write_results_to_file(paths, start_vertex, result_filename, "Dijkstra",false,measure_time,measured_time.count());
                    }
                    if (measure_time) {
                        write_time_info("Dijkstra", measured_time.count());
                    }
                    else {
                        std::cout << "This results were calculated using: Dijkstra algorithm\n\n";
                    }
                    print_path(paths, start_vertex);
                    return 1;
                }
            } else {
                return -1;
            }
        } else if (std::strcmp(algorithm, "B") == 0) {
            if (computing_path_possible(graph)) {
                if (!check_if_starting_vertex_exists(graph,start_vertex)) {
                    std::cerr << "provided vertex does not exists in graph" << std::endl;
                    return -1;
                }
                std::chrono::microseconds measured_time;
                if (measure_time) {
                    auto start = std::chrono::high_resolution_clock::now();
                    paths = bellman_ford(graph, start_vertex);
                    auto end = std::chrono::high_resolution_clock::now();
                    measured_time = to_ms(end - start);
                }
                else {
                    paths = bellman_ford(graph, start_vertex);
                }
                if (paths.first.empty()) {
                    return -1;
                } else {
                    if (write_result_to_file) {
                        write_results_to_file(paths, start_vertex, result_filename, "Bellman-Ford",false,measure_time,measured_time.count());
                    }
                    if (measure_time) {
                        write_time_info("Bellman-Ford", measured_time.count());
                    }
                    else {
                        std::cout << "This results were calculated using: Bellman-Ford algorithm\n\n";
                    }
                    print_path(paths, start_vertex);
                    return 1;
                }
            } else {
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
    if (graph.empty()) {
        return -1;
    }
    std::vector<std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>>> path_set;
    std::vector<std::pair<std::basic_string<char>, int>> paths;
    if (cmd_option_exists(argv, argv+argc, "-a")) {
        if (std::strcmp(algorithm, "D") == 0) {
            if (computing_path_possible(graph)) {
                if (!dijkstra_possible(graph)) {
                    std::cerr << "dijkstra algorithm cannot be applied on this graph" << std::endl;
                    return -1;
                }
                std::chrono::microseconds measured_time;
                if (measure_time) {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (auto const& vertex : graph) {
                        path_set.push_back(dijkstra(graph, vertex.first));
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
                    if (write_result_to_file) {
                        prepare_file_to_appending(result_filename, "Dijkstra", measure_time, measured_time.count());
                    }
                    if (measure_time) {
                        write_time_info("Dijkstra", measured_time.count());
                    }
                    else {
                        std::cout << "This results were calculated using: Dijkstra algorithm\n\n";
                    }
                    for (auto path : path_set) {
                        print_path(path, graph[counter].first);
                        if (write_result_to_file) {
                            write_results_to_file(path, graph[counter].first, result_filename, "Dijkstra", true,
                                                  measure_time, measured_time.count());
                        }
                        std::cout << "\n";
                        counter++;
                    }
                    return 1;
                }
            } else {
                return -1;
            }
        } else if (std::strcmp(algorithm, "B") == 0) {
            if (computing_path_possible(graph)) {
                if (!bellman_ford_possible(graph)) {
                    std::cerr << "bellman-ford algorithm cannot be applied on this graph" << std::endl;
                    return -1;
                }
                std::chrono::microseconds measured_time;
                try {
                    if (measure_time) {
                        auto start = std::chrono::high_resolution_clock::now();
                        for (auto const& vertex : graph) {
                            path_set.push_back(bellman_ford(graph, vertex.first));
                        }
                        auto end = std::chrono::high_resolution_clock::now();
                        measured_time = to_ms(end - start);
                    }
                    else {
                        for (auto const &vertex: graph) {
                            path_set.emplace_back(bellman_ford(graph, vertex.first));
                        }
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << e.what();
                    return -1;
                }
                if (path_set.empty()) {
                    return -1;
                } else {
                    int counter = 0;
                    if (write_result_to_file) {
                        prepare_file_to_appending(result_filename, "Bellman-Ford", measure_time, measured_time.count());
                    }
                    if (measure_time) {
                        write_time_info("Bellman-Ford", measured_time.count());
                    }
                    for (auto path : path_set) {
                        print_path(path, graph[counter].first);
                        if (write_result_to_file) {
                            write_results_to_file(path, graph[counter].first, result_filename, "Bellman-Ford", true,
                                                  measure_time, measured_time.count());
                        }
                        std::cout << "\n";
                        counter++;
                    }
                    return 1;
                }
            } else {
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

