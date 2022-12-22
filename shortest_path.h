#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "custom_utils.h"
#include <queue>
#include <map>


std::vector<std::pair<std::string,std::vector<std::string>>> get_paths(const std::vector<std::pair<std::string,std::string>> &previous_node, const std::string &starting_vertex) {
    std::vector<std::pair<std::string,std::vector<std::string>>> paths;
    for (const auto& vertex : previous_node) {
        if (vertex.first == starting_vertex) {
            paths.push_back({vertex.first, {}});
        }else {
            std::vector<std::string> path;
            auto current = vertex.second;
            while (current != starting_vertex) {
                for (auto const &value: previous_node) {
                    if (value.first == current) {
                        path.emplace_back(current);
                        current = value.second;
                        break;
                    }
                }
            }
            path.push_back(current);
            paths.emplace_back(vertex.first, path);
        }
    }
    return paths;
}

std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>> dijkstra(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph,const std::string &starting_vertex) {
    if (dijkstra_possible(graph)) {
        auto togo = std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<>>();
        std::vector<std::pair<std::string,int>> paths = fill_paths(graph);
        std::vector<std::pair<std::string,std::string>> previous_node = fill_previous_nodes(graph);

        for (auto & path : paths) {
            if (path.first == starting_vertex) {
                path.second = 0;
                togo.emplace(path.second, path.first);
            }
        }

        while (!togo.empty()) {
            auto vertex = togo.top();
            togo.pop();
            for (const auto& val : graph) {
                if (val.first == vertex.second) {
                    for (const auto& neighbour : val.second) {
                        int al = paths[get_index_of_vertex(paths,vertex.second)].second + get_edge_size(graph, vertex.second, neighbour.first);
                        if (al < paths[get_index_of_vertex(paths,neighbour.first)].second) {
                            paths[get_index_of_vertex(paths,neighbour.first)].second = al;
                            previous_node[get_index_of_vertex(paths,neighbour.first)].second = vertex.second;
                            togo.emplace(neighbour.second, neighbour.first);
                        }
                    }
                }
            }
        }

        return {paths,get_paths(previous_node, starting_vertex)};
    } else {
        std::cerr << "dijkstra algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}


std::vector<std::pair<std::string,int>> bellman_ford(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph, const std::string &starting_vertex) {
    if (bellman_ford_possible(graph)) {
        return {};
    } else {
        std::cerr << "bellman-ford algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}