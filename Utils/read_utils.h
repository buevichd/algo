#include <vector>
#include <iostream>

using namespace std;

template <typename T = int>
vector<T> ReadVector(size_t size, istream& in = cin) {
    vector<T> result(size);
    for (size_t i = 0; i < size; ++i) {
        in >> result[i];
    }
    return result;
}

template <typename T = int>
vector<vector<T>> ReadMatrix(size_t height, size_t width, istream& in = cin) {
    vector<vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i] = ReadVector(width, in);
    }
    return result;
}