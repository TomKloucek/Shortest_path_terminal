#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "custom_utils.h"
#include <queue>
/*
 * for each vertex v in Graph.Vertices:
 4          dist[v] ← INFINITY
 5          prev[v] ← UNDEFINED
 6          add v to Q
 7      dist[source] ← 0
 8
 9      while Q is not empty:
10          u ← vertex in Q with min dist[u]
11          remove u from Q
12
13          for each neighbor v of u still in Q:
14              alt ← dist[u] + Graph.Edges(u, v)
15              if alt < dist[v]:
16                  dist[v] ← alt
17                  prev[v] ← u
18
 * */

std::vector<std::pair<std::string,int>> dijkstra(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph, std::string starting_vertex) {
    if (dijkstra_possible(graph)) {
        auto togo = std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<>>();
        std::vector<std::string> visited;
        std::vector<std::pair<std::string,int>> paths = fill_paths(graph);
        std::vector<std::pair<std::string,std::string>> previous_node = fill_previous_nodes(graph);

        for (auto & path : paths) {
            if (path.first == starting_vertex) {
                path.second = 0;
                togo.push({path.second,path.first});
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
        return paths;
    } else {
        std::cerr << "dijkstra algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}

std::vector<std::pair<std::string,int>> bellman_ford(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph, std::string starting_vertex) {
    if (bellman_ford_possible(graph)) {
        return {};
    } else {
        std::cerr << "bellman-ford algorithm cannot be applied on this graph" << std::endl;
        return {};
    }
}