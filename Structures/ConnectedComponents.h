#pragma once

#include <vector>
#include <unordered_set>


// INTERFACE
using Graph = std::vector<std::vector<size_t>>;

using ConnectedComponent = std::unordered_set<size_t>;

std::vector<ConnectedComponent> FindConnectedComponents(const Graph& graph);



// IMPLEMENTATION

class ConnectedComponentFinder {
public:
    explicit ConnectedComponentFinder(const Graph& graph) : graph_(graph), used_(graph.size()) {
    }

    std::vector<ConnectedComponent> Find() {
        std::vector<ConnectedComponent> components;
        for (size_t vertex = 0; vertex < graph_.size(); ++vertex) {
            if (used_[vertex]) {
                continue;
            }
            ConnectedComponent component;
            AddAllConnectedVertexes(vertex, &component);
            components.push_back(component);
        }
        return components;
    }

private:
    const Graph& graph_;
    std::vector<bool> used_;

    void AddAllConnectedVertexes(size_t vertex, ConnectedComponent* component_ptr) {
        component_ptr->insert(vertex);
        used_[vertex] = true;

        for (size_t neighbor : graph_[vertex]) {
            if (used_[neighbor]) {
                continue;
            }
            AddAllConnectedVertexes(neighbor, component_ptr);
        }
    }
};

std::vector<ConnectedComponent> FindConnectedComponents(const Graph& graph) {
    return ConnectedComponentFinder(graph).Find();
}
