#include <vector>
#include <iostream>

using namespace std;

template <typename T = int>
T ReadNumber(istream& in = cin) {
    T number;
    in >> number;
    return number;
}

template <class T>
istream& operator>>(istream& in, vector<T>& elements) {
    for (size_t i = 0; i < elements.size(); ++i) {
        in >> elements[i];
    }
    return in;
}

template <class T, class V>
istream& operator>>(istream& in, pair<T, V> pair) {
    return in >> pair.first >> pair.second;
}

template <typename T = int>
vector<vector<T>> ReadMatrix(size_t height, size_t width, istream& in = cin) {
    vector<vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i] = ReadVector(width, in);
    }
    return result;
}