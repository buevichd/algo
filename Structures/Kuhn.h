#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <numeric>

// INTERFACE

using Graph = std::vector<std::unordered_set<size_t>>;

struct Matching {
    Matching(size_t left_part_index, size_t right_part_index)
        : left_part_index(left_part_index), right_part_index(right_part_index) {
    }

    size_t left_part_index;
    size_t right_part_index;
};

// the most common function
std::vector<Matching> FindMaxMatching(const Graph& bipartite_graph,
                                      const std::vector<size_t>& left_part_vertexes);

// for case when left part consists of first [0, 1, ..., @first_part_size - 1] vertexes
std::vector<Matching> FindMaxMatching(const Graph& bipartite_graph,
                                      size_t first_part_size);

// IMPLEMENTATION

template <class Container, class Element>
bool Contains(const Container& container, const Element& element) {
    return container.find(element) != container.end();
}

class KuhnMaxMatchingFinder {
public:
    KuhnMaxMatchingFinder(const Graph& bipartite_graph,
                          const std::vector<size_t>& left_part_vertexes)
        : graph_(bipartite_graph), left_part_vertexes_(left_part_vertexes), used_(graph_.size()) {
    }

    std::vector<Matching> Find() {
        for (size_t left_vertex : left_part_vertexes_) {
            used_.clear();
            TryExpandMatching(left_vertex);
        }
        std::vector<Matching> result;
        for (const auto& [right_vertex, left_vertex] : right_to_left_matching_) {
            result.emplace_back(left_vertex, right_vertex);
        }
        return result;
    }

    bool TryExpandMatching(size_t left_vertex) {
        if (Contains(used_, left_vertex)) {
            return false;
        }
        used_.insert(left_vertex);
        for (size_t right_vertex : graph_[left_vertex]) {
            if (!Contains(right_to_left_matching_, right_vertex)
                || TryExpandMatching(right_to_left_matching_.at(right_vertex))) {

                right_to_left_matching_[right_vertex] = left_vertex;
                return true;
            }
        }
        return false;
    }

private:
    const Graph& graph_;
    const std::vector<size_t>& left_part_vertexes_;
    std::unordered_map<size_t, size_t> right_to_left_matching_;
    std::unordered_set<size_t> used_;
};

std::vector<Matching> FindMaxMatching(const Graph& bipartite_graph,
                                      const std::vector<size_t>& left_part_vertexes) {
    return KuhnMaxMatchingFinder(bipartite_graph, left_part_vertexes).Find();
}

std::vector<Matching> FindMaxMatching(const Graph& bipartite_graph,
                                      size_t first_part_size) {
    std::vector<size_t> left_part_vertexes(first_part_size);
    std::iota(left_part_vertexes.begin(), left_part_vertexes.end(), 0);
    return FindMaxMatching(bipartite_graph, left_part_vertexes);
}
