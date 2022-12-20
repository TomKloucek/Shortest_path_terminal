#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "custom_utils.h"

std::vector<std::pair<std::string,int>> dijkstra(const std::vector<std::pair<std::string,std::vector<std::pair<std::string,int>>>>& graph) {
    if (dijkstra_possible(graph)) {
        return {};
    } else {
        std::cerr << "dijkstra cannot be applied on this graph";
        return {};
    }
}