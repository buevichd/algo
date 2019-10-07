#include <vector>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <algorithm>

// INTERFACE

using Vertex = size_t;
using Distance = long long;
using Graph = std::vector<std::unordered_map<Vertex, Distance>>;

int Dijkstra(const Graph& graph, Vertex start_vertex, Vertex finish_vertex);

std::vector<Distance> Dijkstra(const Graph& graph, Vertex start_vertex);

// IMPLEMENTATION

struct DijkstraVertex {
    Vertex index;
    Distance distance;
};

class DijkstraVertexHeap {
public:
    void PushVertex(const DijkstraVertex& vertex) {
        nodes_.push_back(vertex);
        size_t node_index = nodes_.size() - 1;
        vertexes_index_to_node_index_[vertex.index] = node_index;
        ShiftUp(node_index);
    }
    
    DijkstraVertex GetMinVertex() {
        return nodes_.front();
    }
    
    void PopMinVertex() {
        SwapNodes(0, nodes_.size() - 1);
        nodes_.pop_back();
        if (nodes_.empty()) {
            return;
        }
        ShiftDown(0);
    }
    
    void DecreaseDistance(Vertex vertex_index, Distance new_distance) {
        size_t node_index = vertexes_index_to_node_index_.at(vertex_index);
        assert(new_distance < nodes_[node_index].distance);
        nodes_[node_index].distance = new_distance;
        ShiftUp(node_index);
    }
    
    bool Empty() {
        return nodes_.empty();
    }
    
    bool Contains(Vertex vertex_index) {
        return vertexes_index_to_node_index_.find(vertex_index) !=
               vertexes_index_to_node_index_.end();
    }
    
    DijkstraVertex GetVertex(Vertex vertex_index) {
        return nodes_[vertexes_index_to_node_index_.at(vertex_index)];
    }

private:
    std::vector<DijkstraVertex> nodes_;
    std::unordered_map<int, size_t> vertexes_index_to_node_index_;
    
    bool HasParent(size_t index) {
        return index != 0;
    }
    
    size_t GetParent(size_t index) {
        assert(index != 0);
        return (index - 1) / 2;
    }
    
    bool HasLeftChild(size_t index) {
        return GetLeftChild(index) < nodes_.size();
    }
    
    size_t GetLeftChild(size_t index) {
        return 2 * index + 1;
    }
    
    bool HasRightChild(size_t index) {
        return GetRightChild(index) < nodes_.size();
    }
    
    size_t GetRightChild(size_t index) {
        return 2 * index + 2;
    }
    
    void SwapNodes(size_t lhs, size_t rhs) {
        std::swap(vertexes_index_to_node_index_.at(nodes_.at(lhs).index),
                  vertexes_index_to_node_index_.at(nodes_.at(rhs).index));
        std::swap(nodes_.at(lhs), nodes_.at(rhs));
    }
    
    bool ShouldSwap(size_t parent, size_t child) {
        return nodes_[parent].distance > nodes_[child].distance;
    }
    
    void ShiftUp(size_t index) {
        while (HasParent(index) && ShouldSwap(GetParent(index), index)) {
            size_t parent_index = GetParent(index);
            SwapNodes(index, parent_index);
            index = parent_index;
        }
    }
    
    void ShiftDown(size_t vertex) {
        while ((HasLeftChild(vertex) && ShouldSwap(vertex, GetLeftChild(vertex))) ||
               (HasRightChild(vertex) && ShouldSwap(vertex, GetRightChild(vertex)))) {
        
            Vertex vertex_to_swap = GetLeftChild(vertex);
            if (HasRightChild(vertex) && ShouldSwap(vertex_to_swap, GetRightChild(vertex))) {
                vertex_to_swap = GetRightChild(vertex);
            }
            SwapNodes(vertex, vertex_to_swap);
            vertex = vertex_to_swap;
        }
    }
};

int Dijkstra(const Graph& graph, Vertex start_vertex, Vertex finish_vertex) {
    return Dijkstra(graph, start_vertex)[finish_vertex];
}

std::vector<Distance> Dijkstra(const Graph& graph, Vertex start_vertex) {
    static constexpr int kInfDistance = -1;
    std::vector<Distance> proved_distances(graph.size(), kInfDistance);
    
    DijkstraVertexHeap reached_vertex_min_heap;
    reached_vertex_min_heap.PushVertex({start_vertex, 0});
    
    while (!reached_vertex_min_heap.Empty()) {
        const DijkstraVertex& dijkstra_vertex = reached_vertex_min_heap.GetMinVertex();
        reached_vertex_min_heap.PopMinVertex();
        proved_distances[dijkstra_vertex.index] = dijkstra_vertex.distance;
        for (const auto& [target_vertex, weight] : graph[dijkstra_vertex.index]) {
            if (proved_distances[target_vertex] != kInfDistance) {
                continue;
            }
            int new_distance = dijkstra_vertex.distance + weight;
            if (!reached_vertex_min_heap.Contains(target_vertex)) {
                reached_vertex_min_heap.PushVertex({target_vertex, new_distance});
            } else if (new_distance < reached_vertex_min_heap.GetVertex(target_vertex).distance) {
                reached_vertex_min_heap.DecreaseDistance(target_vertex, new_distance);
            }
        }
    }
    return proved_distances;
}