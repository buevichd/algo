template <typename T = int>
vector<vector<T>> CreateMatrix(size_t height, size_t width, T placeholder = 0) {
    vector<vector<T>> result(height);
    for (size_t i = 0; i < height; ++i) {
        result[i].assign(width, placeholder);
    }
    return result;
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& elements) {
    for (const T& element : elements) {
        out << element << ' ';
    }
    return out;
}

template <class Container, class Element>
bool Contains(const Container& container, const Element& element) {
    return container.find(element) != container.end();
}