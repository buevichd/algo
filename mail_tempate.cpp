#include <iostream>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <deque>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <functional>

using namespace std;

template <typename T = int>
vector<vector<T>> CreateMatrix(size_t height, size_t width, T placeholder = 0) {
    vector<vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i].assign(width, placeholder);
    }
    return result;
}

template <typename T = int>
T ReadNumber(istream& in = cin) {
    T number;
    in >> number;
    return number;
}

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


int main() {
    ios_base::sync_with_stdio(false);



    return 0;
}