#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using dd = double;
using vi = vector<int>;
using vl = vector<ll>;

template <class T, class V>
istream& operator>>(istream& in, pair<T, V>& pair) {
    return in >> pair.first >> pair.second;
}

template <class T>
istream& operator>>(istream& in, vector<T>& elements) {
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
ostream& operator<<(ostream& out, const vector<T>& elements) {
    for (const T& element : elements) {
        out << element << ' ';
    }
    return out;
}

template <class T, class V>
ostream& operator<<(ostream& out, const pair<T, V>& pair) {
    out << pair.first << ' ' << pair.second;
}

#define FOR(i,n) for(size_t i=0, i##_len=(n); i<i##_len; ++i)
#define SORT(c) sort((c).begin(), (c).end())

int main() {
    ios_base::sync_with_stdio(false);

    
    
    return 0;
}
