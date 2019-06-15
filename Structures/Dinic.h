#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cassert>

// INTERFACE
using Capacity = int64_t;

using Graph = std::vector<std::unordered_map<size_t, Capacity>>;

Graph MakeResidualNetwork(const Graph& graph);

Capacity FindMaxFlowDinic(size_t source, size_t target, const Graph& graph);

Capacity FindMaxFlowDinicForResidualNetwork(size_t source, size_t target, Graph* residual_network);

// pair represents edge (from -> to)
std::vector<std::pair<size_t, size_t>> FindMinFlowEdgeCut(size_t source, size_t target,
                                                          const Graph& graph, Capacity* flow);

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

std::vector<int> MakeLevels(const Graph& graph, size_t source) {
    size_t vertex_count = graph.size();
    std::vector<int> levels(vertex_count, -2);

    std::queue<size_t> vertex_queue;
    std::vector<bool> used(vertex_count);
    std::vector<size_t> found_by(vertex_count);

    vertex_queue.push(source);
    used[source] = true;
    found_by[source] = source;
    levels[source] = -1;

    while (!vertex_queue.empty()) {
        size_t vertex = vertex_queue.front();
        vertex_queue.pop();

        levels[vertex] = levels[found_by[vertex]] + 1;

        for (const auto& [neighbour, capacity] : graph[vertex]) {
            if (used[neighbour] || capacity == 0) {
                continue;
            }
            used[neighbour] = true;
            found_by[neighbour] = vertex;
            vertex_queue.push(neighbour);
        }
    }

    return levels;
}

Graph MakeLayeredNetwork(const Graph& graph, size_t source) {
    std::vector<int> levels = MakeLevels(graph, source);

    Graph layered_network(graph.size());
    for (size_t from = 0; from < graph.size(); ++from) {
        for (const auto& [to, capacity] : graph[from]) {
            if (levels[from] + 1 == levels[to]) {
                layered_network[from][to] = capacity;
            }
        }
    }
    return layered_network;
}

class MaxFlowDinicFinder {
public:
    explicit MaxFlowDinicFinder(Graph& residual_network)
        : residual_network_(residual_network) {
    }

    Capacity Find(size_t source, size_t target) {
        Capacity flow = 0;
        Capacity addend_flow;
        while ((addend_flow = FindMaxBlockingFlow(source, target)) != 0) {
            flow += addend_flow;
        }
        return flow;
    }

private:
    using EdgeIterator = std::unordered_map<size_t, Capacity>::iterator;

    Graph& residual_network_;
    Graph layered_network_;
    std::vector<EdgeIterator> unvisited_edges_;

    Capacity FindMaxBlockingFlow(size_t source, size_t target) {
        layered_network_ = MakeLayeredNetwork(residual_network_, source);
        unvisited_edges_ = MakeUnvisitedEdges();
        return FindFlow(source, target);
    }

    Capacity FindFlow(size_t current, size_t target, Capacity input_flow = INT32_MAX) {
        if (current == target) {
            return input_flow;
        }
        Capacity flow = 0;
        for (EdgeIterator& edge_iter = unvisited_edges_.at(current);
             edge_iter != layered_network_.at(current).end();
             ++edge_iter) {

            const auto& [next_vertex, capacity] = *edge_iter;
            if (capacity == 0) {
                continue;
            }
            Capacity next_input_flow = std::min(capacity, input_flow);
            Capacity addend_flow = FindFlow(next_vertex, target, next_input_flow);

            flow += addend_flow;
            input_flow -= addend_flow;

            UpdateEdge(current, next_vertex, addend_flow, &layered_network_);
            UpdateEdge(current, next_vertex, addend_flow, &residual_network_);

            if (input_flow == 0) {
                break;
            }
        }
        return flow;
    }

    void UpdateEdge(size_t from, size_t to, Capacity addend_flow, Graph* graph) {
        graph->at(from).at(to) -= addend_flow;

        if (Contains(graph->at(to), from)) {
            graph->at(to).at(from) += addend_flow;
        }
    }

    std::vector<EdgeIterator> MakeUnvisitedEdges() {
        std::vector<EdgeIterator> unvisited_edges;
        for (auto& vertex_edges : layered_network_) {
            unvisited_edges.push_back(vertex_edges.begin());
        }
        return unvisited_edges;
    }
};

Capacity FindMaxFlowDinic(size_t source, size_t target, const Graph& graph) {
    Graph residual_network = MakeResidualNetwork(graph);
    return FindMaxFlowDinicForResidualNetwork(source, target, &residual_network);
}


Capacity FindMaxFlowDinicForResidualNetwork(size_t source, size_t target, Graph* residual_network) {
    return MaxFlowDinicFinder(*residual_network).Find(source, target);
}

class MinFlowEdgeCutFinder {
public:
    explicit MinFlowEdgeCutFinder(const Graph& graph)
        : graph_(graph),
          residual_network_(MakeResidualNetwork(graph_)),
          reachable_from_source_(graph.size()) {
    }

    std::vector<std::pair<size_t, size_t>> Find(size_t source, size_t target, Capacity* flow) {
        Capacity max_flow = FindMaxFlowDinicForResidualNetwork(source, target, &residual_network_);
        *flow = max_flow;

        FindAllReachableFromVertex(source);
        assert(!reachable_from_source_[target]);

        Capacity edge_cut_flow = 0;
        std::vector<std::pair<size_t, size_t>> cut_edges;
        for (size_t vertex = 0; vertex < graph_.size(); ++vertex) {
            if (!reachable_from_source_[vertex]) {
                continue;
            }
            for (const auto& [to, capacity] : residual_network_[vertex]) {
                if (reachable_from_source_[to]) {
                    continue;
                }
                assert(capacity == 0);
                if (!Contains(graph_[vertex], to)) {
                    continue;
                }
                Capacity addend_flow = residual_network_[to].at(vertex);
                assert(addend_flow == graph_[vertex].at(to));
                edge_cut_flow += addend_flow;
                cut_edges.emplace_back(vertex, to);
            }
        }
        assert(max_flow == edge_cut_flow);
        return cut_edges;
    }

private:
    void FindAllReachableFromVertex(size_t vertex) {
        assert(!reachable_from_source_[vertex]);
        reachable_from_source_[vertex] = true;
        for (const auto& [to, capacity] : residual_network_[vertex]) {
            if (reachable_from_source_[to] || capacity == 0) {
                continue;
            }
            FindAllReachableFromVertex(to);
        }
    }

private:
    const Graph& graph_;
    Graph residual_network_;
    std::vector<bool> reachable_from_source_;
};

std::vector<std::pair<size_t, size_t>> FindMinFlowEdgeCut(size_t source, size_t target,
                                                          const Graph& graph, Capacity* flow) {

    return MinFlowEdgeCutFinder(graph).Find(source, target, flow);
}