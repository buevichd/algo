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
#include <limits>

using ll = long long;
using dd = double;

using namespace std;

template <class T>
istream& operator>>(istream& in, vector<T>& elements) {
    for (size_t i = 0; i < elements.size(); ++i) {
        in >> elements[i];
    }
    return in;
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