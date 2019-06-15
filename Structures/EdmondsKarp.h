#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>
#include <cassert>
#include <algorithm>

// ----------------------------------------------------------------------------
// INTERFACE

using Capacity = int;

using Graph = std::vector<std::unordered_map<size_t, Capacity>>;

Graph MakeResidualNetwork(const Graph& graph);

// NOTE: residual network is required, use MakeResidualNetwork for your graph if needed
Capacity FindMaxFlowEdmondsKarp(size_t source, size_t target, Graph residual_network);

// ----------------------------------------------------------------------------
// IMPLEMENTATION

// for debugging
void PrintGraph(const Graph& graph) {
    std::cout << "Graph:" << std::endl;
    for (size_t vertex = 0; vertex < graph.size(); ++vertex) {
        std::cout << vertex << " -> ";
        for (const auto& [to, capacity] : graph[vertex]) {
            std::cout << "(" << to << ", " << capacity << "), ";
        }
        std::cout << std::endl;
    }
}

template <class Container, class Element>
bool Contains(const Container& container, const Element& element) {
    return container.find(element) != container.end();
}

Graph MakeResidualNetwork(const Graph& graph) {
    Graph residual_network(graph.size());
    for (size_t from = 0; from < graph.size(); ++from) {
        for (const auto& [to, capacity] : graph[from]) {
            residual_network[from][to] = capacity;
            if (!Contains(graph[to], from)) {
                residual_network[to][from] = 0;
            }
        }
    }
    return residual_network;
}

struct VertexInfo {
    bool used;
    size_t previous_vertex;
};

Capacity ApplyAddendFlow(size_t source, size_t target, const std::vector<VertexInfo>& vertex_infos,
                         Graph* residual_network_ptr) {

    Graph& residual_network = *residual_network_ptr;
    Capacity min_capacity = residual_network[vertex_infos[target].previous_vertex][target];
    for (size_t vertex = target; vertex != source; vertex = vertex_infos[vertex].previous_vertex) {
        size_t from = vertex_infos[vertex].previous_vertex;
        size_t to = vertex;
        min_capacity = std::min(min_capacity, residual_network[from][to]);
    }
    assert(min_capacity > 0);
    for (size_t vertex = target; vertex != source; vertex = vertex_infos[vertex].previous_vertex) {
        size_t from = vertex_infos[vertex].previous_vertex;
        size_t to = vertex;
        residual_network[from][to] -= min_capacity;
        residual_network[to][from] += min_capacity;
    }
    return min_capacity;
}

bool FindAddendFlow(size_t source, size_t target, Graph* residual_network_ptr,
                    Capacity* addend_flow) {

    Graph& residual_network = *residual_network_ptr;
    std::queue<size_t> vertex_queue;
    std::vector<VertexInfo> vertex_infos(residual_network.size());

    vertex_queue.push(source);
    vertex_infos[source].used = true;

    while (!vertex_queue.empty()) {
        size_t from = vertex_queue.front();
        vertex_queue.pop();

        for (const auto& [to, capacity] : residual_network[from]) {
            if (vertex_infos[to].used || capacity == 0) {
                continue;
            }
            vertex_infos[to] = {true, from};
            if (to == target) {
                *addend_flow = ApplyAddendFlow(source, target, vertex_infos, residual_network_ptr);
                return true;
            }
            vertex_queue.push(to);
        }
    }
    return false;
}

Capacity FindMaxFlowEdmondsKarp(size_t source, size_t target, Graph residual_network) {
    Capacity flow = 0;
    Capacity addend_flow;
    while (FindAddendFlow(source, target, &residual_network, &addend_flow)) {
        flow += addend_flow;
    }
    return flow;
}
