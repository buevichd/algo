#include <vector>

template <typename T = int>
vector<vector<T>> CreateMatrix(size_t height, size_t width, T placeholder = 0) {
    vector<vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i].assign(width, placeholder);
    }
    return result;
}