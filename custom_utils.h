#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>

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
    char ** itr = std::find(begin, end, option);
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