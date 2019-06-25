#include <vector>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <algorithm>

// INTERFACE

struct Edge {
    int target_vertex;
    int weight;
};

using Graph = std::vector<std::vector<Edge>>;

int Dijkstra(const Graph* graph, int start_vertex, int finish_vertex);

// IMPLEMENTATION

struct DijkstraVertex {
    int index;
    int distance;
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
    
    void DecreaseDistance(int vertex_index, int new_distance) {
        size_t node_index = vertexes_index_to_node_index_.at(vertex_index);
        assert(new_distance < nodes_[node_index].distance);
        nodes_[node_index].distance = new_distance;
        ShiftUp(node_index);
    }
    
    bool Empty() {
        return nodes_.empty();
    }
    
    bool Contains(int vertex_index) {
        return vertexes_index_to_node_index_.find(vertex_index) !=
               vertexes_index_to_node_index_.end();
    }
    
    DijkstraVertex GetVertex(int vertex_index) {
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
    
    void ShiftDown(size_t index) {
        size_t index_to_swap;
        do {
            index_to_swap = index;
            if (HasLeftChild(index) && ShouldSwap(index, GetLeftChild(index))) {
                index_to_swap = GetLeftChild(index);
            }
            if (HasRightChild(index) && ShouldSwap(index_to_swap, GetRightChild(index))) {
                index_to_swap = GetRightChild(index);
            }
            if (index == index_to_swap) {
                break;
            }
            SwapNodes(index, index_to_swap);
            index = index_to_swap;
        } while (index != index_to_swap);
    }
};

int Dijkstra(const Graph* graph, int start_vertex, int finish_vertex) {
    static constexpr int kInfDistance = -1;
    std::vector<int> proved_distances(graph->size(), kInfDistance);
    
    DijkstraVertexHeap reached_vertex_min_heap;
    reached_vertex_min_heap.PushVertex({start_vertex, 0});
    
    while (!reached_vertex_min_heap.Empty()) {
        const DijkstraVertex& dijkstra_vertex = reached_vertex_min_heap.GetMinVertex();
        reached_vertex_min_heap.PopMinVertex();
        if (dijkstra_vertex.index == finish_vertex) {
            return dijkstra_vertex.distance;
        }
        proved_distances[dijkstra_vertex.index] = dijkstra_vertex.distance;
        for (const Edge& edge : (*graph)[dijkstra_vertex.index]) {
            int target_vertex = edge.target_vertex;
            if (proved_distances[target_vertex] != kInfDistance) {
                continue;
            }
            int new_distance = dijkstra_vertex.distance + edge.weight;
            if (!reached_vertex_min_heap.Contains(target_vertex)) {
                reached_vertex_min_heap.PushVertex({target_vertex, new_distance});
            } else if (new_distance < reached_vertex_min_heap.GetVertex(target_vertex).distance) {
                reached_vertex_min_heap.DecreaseDistance(target_vertex, new_distance);
            }
        }
    }
    return -1;
}