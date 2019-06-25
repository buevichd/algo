#include <bits/stdc++.h>

// USEFUL MERGE FUNCTORS
template <class T>
std::function<T(T, T)> sum_merge_functor = [](T left, T right) {
    return left + right;
};

template <class T>
std::function<T(T, T)> max_merge_functor = [](T left, T right) {
    return std::max(left, right);
};

template <class T>
std::function<T(T, T)> min_merge_functor = [](T left, T right) {
    return std::min(left, right);
};

// SEGMENT TREE
template<class T = int>
class SimpleSegmentTree {
public:
    SimpleSegmentTree(std::vector<T> data, const std::function<T(T, T)>& merge_functor, T identity = 0)
        : identity_(identity),
          data_(std::move(data)),
          tree_(4 * data_.size()),
          addend_(4 * data_.size()),
          merge_functor_(merge_functor) {

        if (!data_.empty()) {
            Build(0, 0, data_.size());
        }
    }

    T GetValue(size_t left, size_t right = SIZE_MAX) {
        if (right == SIZE_MAX) {
            right = left + 1;
        }
        assert(left < data_.size());
        assert(right <= data_.size());
        return GetValue(0, 0, data_.size(), left, right);
    }

    void SetElement(size_t position, T new_value) {
        assert(position < data_.size());
        SetElement(0, 0, data_.size(), position, new_value);
    }

    void UpdateElement(size_t position, T addend) {
        assert(position < data_.size());
        SetElement(position, GetValue(position) + addend);
    }

private:
    void Build(size_t vertex, size_t left, size_t right) {
        if (left + 1 == right) {
            tree_[vertex] = data_[left];
        } else {
            size_t middle = (right + left) / 2;
            Build(GetLeftChild(vertex), left, middle);
            Build(GetRightChild(vertex), middle, right);
            tree_[vertex] = merge_functor_(tree_[GetLeftChild(vertex)], tree_[GetRightChild(vertex)]);
        }
    }

    T GetValue(size_t vertex, size_t vertex_left, size_t vertex_right, size_t left, size_t right) {
        if (left >= right) {
            return identity_;
        }
        if (vertex_left == left && vertex_right == right) {
            return tree_[vertex];
        }
        size_t vertex_middle = (vertex_left + vertex_right) / 2;
        return merge_functor_(
                GetValue(GetLeftChild(vertex), vertex_left, vertex_middle, left, std::min(vertex_middle, right)),
                GetValue(GetRightChild(vertex), vertex_middle, vertex_right, std::max(left, vertex_middle), right));
    }

    void SetElement(size_t vertex, size_t left, size_t right, size_t position, T new_value) {
        if (left + 1 == right) {
            tree_[vertex] = new_value;
        } else {
            size_t middle = (left + right) / 2;
            if (position < middle) {
                SetElement(GetLeftChild(vertex), left, middle, position, new_value);
            } else {
                SetElement(GetRightChild(vertex), middle, right, position, new_value);
            }
            tree_[vertex] = merge_functor_(tree_[GetRightChild(vertex)], tree_[GetLeftChild(vertex)]);
        }
    }

    size_t GetLeftChild(size_t vertex) const {
        return 2 * vertex + 1;
    }

    size_t GetRightChild(size_t vertex) const {
        return 2 * vertex + 2;
    }

    T identity_;
    std::vector<T> data_;
    std::vector<T> tree_;
    std::vector<T> addend_;
    std::function<T(T, T)> merge_functor_;
};
