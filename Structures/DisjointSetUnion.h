#pragma once

#include <cstddef>
#include <vector>

using namespace std;

template <class T = int>
class DisjointSetUnion {
public:
    explicit DisjointSetUnion(size_t size) : parent_(size), rank_(size), sets_count_(size) {
        for (T i = 0; static_cast<size_t>(i) < size; ++i) {
            parent_[i] = i;
        }
    }

    T FindSet(T element) {
        if (element == parent_[element]) {
            return element;
        }
        return parent_[element] = FindSet(parent_[element]);
    }

    void UnionSets(T first, T second) {
        first = FindSet(first);
        second = FindSet(second);
        if (first != second) {
            --sets_count_;
            if (rank_[first] < rank_[second]) {
                swap(first, second);
            }
            parent_[second] = first;
            if (rank_[first] == rank_[second]) {
                ++rank_[first];
            }
        }
    }

    bool IsInSameSet(T lhs, T rhs) {
        return FindSet(lhs) == FindSet(rhs);
    }

    size_t GetSetsCount() const {
        return sets_count_;
    }

private:
    vector<T> parent_;
    vector<int> rank_;
    size_t sets_count_;
};
