#include <vector>
#include <iostream>

template <class T, class V>
std::istream& operator>>(std::istream& in, std::pair<T, V>& pair) {
    return in >> pair.first >> pair.second;
}

template <class T>
std::istream& operator>>(std::istream& in, std::vector<T>& elements) {
    for (size_t i = 0; i < elements.size(); ++i) {
        in >> elements[i];
    }
    return in;
}

template <typename T = int>
std::vector<std::vector<T>> ReadMatrix(size_t height, size_t width, std::istream& in = std::cin) {
    std::vector<std::vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i] = ReadVector(width, in);
    }
    return result;
}