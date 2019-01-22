#include <functional>
#include <vector>
#include <unordered_map>

template <class ElementType, class KeyType, class Compare = std::less<ElementType>>
class MaxHeap {
public:
    explicit MaxHeap(Compare compare) : compare_(compare) {
    }

    ElementType Top() const {
        return elements_.front();
    }

    KeyType TopKey() const {
        return keys_.front();
    }

    void Pop() {
        Swap(0, elements_.size() - 1);
        EraseLast();
        ShiftDown(0);
    }

    void Add(KeyType key, ElementType element) {
        elements_.push_back(std::move(element));
        keys_.push_back(key);
        Vertex vertex = elements_.size() - 1;
        key_to_vertex_[key] = vertex;
        ShiftUp(vertex);
    }

    ElementType At(KeyType key) const {
        return elements_[key_to_vertex_.at(key)];
    }

    void Erase(KeyType key) {
        Vertex vertex = key_to_vertex_.at(key);
        Vertex last_vertex = elements_.size() - 1;
        Swap(vertex, last_vertex);
        EraseLast();
        if (vertex != last_vertex) {
            ShiftDown(vertex);
            ShiftUp(vertex);
        }
    }

    bool Contains(KeyType key) {
        return key_to_vertex_.find(key) != key_to_vertex_.end();
    }

    bool Empty() {
        return elements_.empty();
    }

    size_t Size() const {
        return elements_.size();
    }

private:
    using Vertex = size_t;

    std::vector<ElementType> elements_;
    std::vector<KeyType> keys_;
    std::unordered_map<KeyType, Vertex> key_to_vertex_;
    Compare compare_;

    void EraseLast() {
        key_to_vertex_.erase(keys_[elements_.size() - 1]);
        keys_.pop_back();
        elements_.pop_back();
    }

    void ShiftUp(Vertex vertex) {
        while (HasParent(vertex) && ShouldSwap(GetParent(vertex), vertex)) {
            Swap(GetParent(vertex), vertex);
            vertex = GetParent(vertex);
        }
    }

    void ShiftDown(Vertex vertex) {
        while ((HasLeftChild(vertex) && ShouldSwap(vertex, GetLeftChild(vertex))) ||
               (HasRightChild(vertex) && ShouldSwap(vertex, GetRightChild(vertex)))) {

            Vertex vertex_to_swap = GetLeftChild(vertex);
            if (HasRightChild(vertex) && ShouldSwap(vertex_to_swap, GetRightChild(vertex))) {
                vertex_to_swap = GetRightChild(vertex);
            }
            Swap(vertex, vertex_to_swap);
            vertex = vertex_to_swap;
        }
    }

    bool HasParent(Vertex vertex) const {
        return vertex != 0;
    }

    Vertex GetParent(Vertex vertex) const {
        return (vertex - 1) / 2;
    }

    bool HasLeftChild(Vertex vertex) const {
        return GetLeftChild(vertex) < elements_.size();
    }

    Vertex GetLeftChild(Vertex vertex) const {
        return 2 * vertex + 1;
    }

    bool HasRightChild(Vertex vertex) const {
        return GetRightChild(vertex) < elements_.size();
    }

    Vertex GetRightChild(Vertex vertex) const {
        return 2 * vertex + 2;
    }

    bool ShouldSwap(Vertex parent, Vertex child) const {
        return compare_(elements_[parent], elements_[child]);
    }

    void Swap(Vertex lhs, Vertex rhs) {
        std::swap(elements_[lhs], elements_[rhs]);
        std::swap(key_to_vertex_[keys_[lhs]], key_to_vertex_[keys_[rhs]]);
        std::swap(keys_[lhs], keys_[rhs]);
    }
};