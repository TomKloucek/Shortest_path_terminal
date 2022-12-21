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

std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>> prepare_file_for_graph(std::ifstream &file) {
    std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>> graph;
    std::string line;
    while (getline (file, line)) {
        std::pair<std::string,std::vector<std::pair<std::string,int>>> vertex;
        std::vector<std::string> vertex_value = split(line,':');
        vertex.first = vertex_value[0];

        std::vector<std::string> neighbours = split(vertex_value[1],',');
        for (const auto& neighbour : neighbours) {
            auto edge = split(neighbour,'=');
            vertex.second.emplace_back(edge[0], std::stoi(edge[1]));
        }
        graph.emplace_back(vertex);
    }
    file.close();
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
                    std::cout << target;
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
        paths.emplace_back(vertex.first, INT32_MAX);
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

bool computing_path_possible(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    return all_vertex_names_used_once(graph);
}

void print_path(const std::vector<std::pair<std::string,int>>& path, const std::string& starting_vertex) {
    std::cout << "Path for vertex: " << starting_vertex << "\n";
    for (const auto& vertex : path) {
        std::cout << "| " << vertex.first << " | " << std::to_string(vertex.second) << " |" << "\n";
    }
    std::cout << std::endl;
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

bool check_if_starting_vertex_exists(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph,const std::string& source) {
    return std::any_of(graph.begin(), graph.end(), [source](auto vertex) { return vertex.first == source;});
}

void write_results_to_file(const std::vector<std::pair<std::string,int>>& path, const std::string& starting_vertex, const std::string& filename, const std::string& algorithm, bool measured=false,int millisecond_took=0) {
    std::ofstream file;
    file.open (filename);
    file << "This results were calculated using: " << algorithm << " algorithm\n";
    if (measured) {
        file << "Computations needed: " << millisecond_took << "ms to finish\n";
    }
    file << "Path for vertex: " << starting_vertex << "\n";
    for (const auto& vertex : path) {
        file << "| " << vertex.first << " | " << std::to_string(vertex.second) << " |" << "\n";
    }
    file << std::endl;
    file.close();
}