#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

char* get_cmd_option(char ** begin, char ** end, const std::string & option)
{
    auto ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool cmd_option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

bool parameters_are_right(char **argv, int argc) {
    std::vector<std::string> possible_flags = {"--help","-s","-f","-w","-t","-a"};
    bool waiting_for_value = false;
    for(int i = 1; i < argc; ++i) {
        if (std::find(possible_flags.begin(), possible_flags.end(), argv[i]) != possible_flags.end()) {
            if (std::strcmp(argv[i], "-t") == 0) {
                continue;
            } else {
                waiting_for_value = true;
            }
        } else {
            if (waiting_for_value) {
                waiting_for_value = false;
            } else {
                std::cerr << "invalid option: " << argv[i] << "\n";
                return false;
            }
        }
    }
    return true;
}

std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>> prepare_file_for_graph(std::ifstream &file) {
    std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>> graph;
    std::string line;
    try {
        while (getline(file, line)) {
            std::pair<std::string, std::vector<std::pair<std::string, int>>> vertex;
            std::vector<std::string> vertex_value = split(line, ':');
            vertex.first = vertex_value[0];

            std::vector<std::string> neighbours = split(vertex_value[1], ',');
            for (const auto &neighbour: neighbours) {
                auto edge = split(neighbour, '=');
                vertex.second.emplace_back(edge[0], std::stoi(edge[1]));
            }
            graph.emplace_back(vertex);
        }
        file.close();
    } catch (std::exception e) {
        std::cout << "input file did not meet format requirements" << std::endl;
        return {};
    }
    return graph;
}

void print_graph(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    for (const auto& vertex : graph) {
        std::string neighbours;
        for (const auto& neighbour : vertex.second) {
            neighbours += "("+neighbour.first+":"+std::to_string(neighbour.second)+"),";
        }
        neighbours.pop_back();
        std::cout << vertex.first << " -> " << neighbours << "\n";
    }
    std::cout << std::endl;
}

bool dijkstra_possible(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    for (const auto& vertex : graph) {
        std::string neighbours;
        for (const auto& neighbour : vertex.second) {
            if (neighbour.second <= 0) {
                return false;
            }
        }
    }
    return true;
}


bool graph_is_undirected(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    for (const auto& checked : graph) {
        std::string target = checked.first;
        for (const auto& vertex : graph) {
            if (vertex.first != target) {
                bool found = false;
                for (const auto& neighbour : vertex.second) {
                    if (neighbour.first == target) {
                        found = true;
                    }
                }
                if (!found) {
                    return false;
                }
            }
        }
    }
    return true;
}


bool bellman_ford_possible(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    return (graph_is_undirected(graph) && dijkstra_possible(graph)) || !(graph_is_undirected(graph));
}

std::vector<std::pair<std::string,int>> fill_paths(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    std::vector<std::pair<std::string,int>> paths;
    for (const auto& vertex : graph) {
        paths.emplace_back(vertex.first, std::numeric_limits<int>::max());
    }
    return paths;
}

std::vector<std::pair<std::string,std::string>> fill_previous_nodes(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    std::vector<std::pair<std::string,std::string>> previous;
    for (const auto& vertex : graph) {
        previous.emplace_back(vertex.first, "");
    }
    return previous;
}

bool all_vertex_names_used_once(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    std::set<std::string> check_set;
    for (const auto& vertex : graph) {
        check_set.insert(vertex.first);
    }
    return check_set.size() == graph.size();
}

bool contains_not_existing_edges(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    std::set<std::string> check_set;
    for (const auto& vertex : graph) {
        check_set.insert(vertex.first);
    }
    for (auto const& vertex : graph) {
        for (auto const& neighbour : vertex.second) {
            if (check_set.find(neighbour.first) == check_set.end()) {
                return true;
            }
        }
    }
    return false;
}

bool check_if_starting_vertex_exists(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph,const std::string& source) {
    return std::any_of(graph.begin(), graph.end(), [source](auto vertex) { return vertex.first == source;});
}

bool computing_path_possible(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    if (!all_vertex_names_used_once(graph)) {
        std::cerr << "provided graph does not meet one of the following rules: all vertex names can be used only once" << std::endl;
        return false;
    }
    if (contains_not_existing_edges(graph)) {
        std::cerr << "provided graph does not meet one of the following rule: edges contains non existent target" << std::endl;
        return false;
    }
    return true;
}

size_t find_largest_number_size(const std::vector<std::pair<std::string,int>>& path) {
    size_t max = 0;
    for (auto const& vertex : path) {
        if (std::to_string(vertex.second).size() > max) {
            max = std::to_string(vertex.second).size();
        }
    }
    return max;
}

void remove_last_n_characters_from_string(int n, std::string& result) {
    for (int i = 0; i < n; ++i) {
        result.pop_back();
    }
}

void print_path(std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>>& path, const std::string& starting_vertex) {
    std::cout << "Shortest path from vertex: " << starting_vertex << "\n";
    size_t max_size = find_largest_number_size(path.first);
    for (const auto& vertex : path.first) {

        std::cout << "| " << vertex.first << " | ";
        for (int i = 0; i < (max_size-std::to_string(vertex.second).size()); ++i) {
            std::cout << " ";
        }
        std::cout << std::to_string(vertex.second) << " |";
        std::string path_string = " " +vertex.first + " ->";
        for (auto const& element : path.second) {
            if (element.first == vertex.first) {
                for (auto const& val: element.second) {
                    path_string += " " + val + " ->";
                }
                remove_last_n_characters_from_string(3, path_string);
            }
        }
        std::cout << path_string << "\n";
    }
    std::cout << std::endl;
}

void write_time_info(const std::string& algorithm, int millisecond_took=0) {
    std::cout << "This results were calculated using: " << algorithm << " algorithm\n";
    std::cout << "Computations needed: " << millisecond_took << " ms to finish\n\n";
}

int get_index_of_vertex(const std::vector<std::pair<std::string,int>>& path, const std::string& vertex) {
    for (int i = 0; i < path.size(); ++i) {
        if (vertex == path[i].first) {
            return i;
        }
    }
    return -1;
}

int get_edge_size(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph, const std::string& source, const std::string& target) {
    for (const auto& vertex : graph) {
        if (vertex.first == source) {
            for (const auto& neighbour : vertex.second) {
                if (neighbour.first == target) {
                    return neighbour.second;
                }
            }
        }
    }
    return 0;
}

void prepare_file_to_appending(const std::string& filename,  const std::string& algorithm, bool measured=false,int millisecond_took=0) {
    std::ofstream ofs;
    ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open(filename);
    ofs << "This results were calculated using: " << algorithm << " algorithm\n";
    if (measured) {
        ofs << "Computations needed: " << millisecond_took << "ms to finish\n\n";
    }
}

void write_results_to_file(std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>>& path, const std::string& starting_vertex, const std::string& filename, const std::string& algorithm,bool append, bool measured=false,int millisecond_took=0) {
    std::ofstream file;
    if (append) {
        file.open(filename,std::ios_base::app);
    } else {
        file.open(filename);
    }
    if (!append) {
        file << "This results were calculated using: " << algorithm << " algorithm\n";
        if (measured) {
            file << "Computations needed: " << millisecond_took << "ms to finish\n";
        }
    }
    file << "Shortest paths from vertex : " << starting_vertex << "\n";
    size_t max_size = find_largest_number_size(path.first);
    for (const auto& vertex : path.first) {

        file << "| " << vertex.first << " | ";
        for (int i = 0; i < (max_size-std::to_string(vertex.second).size()); ++i) {
            file << " ";
        }
        file << std::to_string(vertex.second) << " |";
        std::string path_string = " " +vertex.first + " ->";
        for (auto const& element : path.second) {
            if (element.first == vertex.first) {
                for (auto const& val: element.second) {
                    path_string += " " + val + " ->";
                }
                remove_last_n_characters_from_string(3, path_string);
            }
        }
        file << path_string << "\n";
    }
    file << std::endl;
    file.close();
}