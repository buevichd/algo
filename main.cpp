#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using dd = double;
using vi = vector<int>;

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

template <class Container, class Element>
bool Contains(const Container& container, const Element& element) {
    return container.find(element) != container.end();
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& elements) {
    for (const T& element : elements) {
        out << element << ' ';
    }
    return out;
}

int main() {
    ios_base::sync_with_stdio(false);



    return 0;
}
