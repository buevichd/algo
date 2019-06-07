#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using dd = double;
using vi = vector<int>;

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

template <class Container, class Element>
bool Contains(const Container& container, const Element& element) {
    return container.find(element) != container.end();
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& elements) {
    for (const T& element : elements) {
        out << element << ' ';
    }
    return out;
}

#define FOR(i,n) for(size_t i=0, i##_len=(n); i<i##_len; ++i)

int main() {
    ios_base::sync_with_stdio(false);



    return 0;
}
