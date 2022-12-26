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
                for (auto const &value : previous_node) {
                    if (value.first == current) {
                        path.emplace_back(current);
                        current = value.second;
                        break;
                    }
                }
                if (current.empty()) {
                    break;
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
        std::vector<std::pair<std::string,int>> distances = fill_paths(graph);
        std::vector<std::pair<std::string,std::string>> previous_node = fill_previous_nodes(graph);

        for (auto & path : distances) {
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
                        int al = distances[get_index_of_vertex(distances, vertex.second)].second + get_edge_size(graph, vertex.second, neighbour.first);
                        if (al < distances[get_index_of_vertex(distances, neighbour.first)].second) {
                            distances[get_index_of_vertex(distances, neighbour.first)].second = al;
                            previous_node[get_index_of_vertex(distances, neighbour.first)].second = vertex.second;
                            togo.emplace(neighbour.second, neighbour.first);
                        }
                    }
                }
            }
        }

        return {distances, get_paths(previous_node, starting_vertex)};
    } else {
        std::cerr << "dijkstra algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}

std::vector<std::pair<int, std::pair<std::string, std::string>>> get_all_edges(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    std::vector<std::pair<int, std::pair<std::string, std::string>>> edges;
    for (const auto& vertex : graph) {
        for (const auto& edge : vertex.second) {
            edges.push_back({edge.second,{vertex.first,edge.first}});
        }
    }
    return edges;
}

int get_edge_distance(std::vector<std::pair<std::string,int>> distances, std::string from) {
    for (auto const& vertex : distances) {
        if (vertex.first == from) {
            return vertex.second;
        }
    }
    return INT32_MIN;
}

std::pair<std::vector<std::pair<std::string,int>>,std::vector<std::pair<std::string,std::vector<std::string>>>> bellman_ford(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph, const std::string &starting_vertex) {
    if (bellman_ford_possible(graph)) {
        std::vector<std::pair<std::string,int>> distances = fill_paths(graph);
        std::vector<std::pair<std::string,std::string>> previous_nodes = fill_previous_nodes(graph);

        for (auto & distance : distances) {
            if (distance.first == starting_vertex) {
                distance.second = 0;
            }
        }

        for (int i = 0; i < (graph.size()-1); ++i) {
            for (auto const & edge : get_all_edges(graph)) {
                int u_dist = get_edge_distance(distances, edge.second.first);
                int v_dist = get_edge_distance(distances, edge.second.second);
                if (u_dist != std::numeric_limits<int>::max()) {
                    if (u_dist + edge.first < v_dist) {
                        for (auto &dist: distances) {
                            if (dist.first == edge.second.second) {
                                dist.second = u_dist + edge.first;
                            }
                        }
                        for (auto &predecessor: previous_nodes) {
                            if (predecessor.first == edge.second.second) {
                                predecessor.second = edge.second.first;
                            }
                        }
                    }
                }
            }
        }
        for (auto const & edge : get_all_edges(graph)) {
            int u_dist = get_edge_distance(distances, edge.second.first);
            int v_dist = get_edge_distance(distances, edge.second.second);
            if (u_dist != std::numeric_limits<int>::max()) {
                if (u_dist + edge.first < v_dist) {
                    throw std::invalid_argument("received graph with negative cycle");
                }
            }
        }
        return {distances, get_paths(previous_nodes, starting_vertex)};
    } else {
        std::cerr << "bellman-ford algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}