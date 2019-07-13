int64_t InverseMod(int64_t number, int64_t mod);

int64_t Plus(int64_t a, int64_t b, int64_t mod) {
    return (a + b) % mod;
}

int64_t Minus(int64_t a, int64_t b, int64_t mod) {
    return (a + mod - b) % mod;
}

int64_t Mult(int64_t a, int64_t b, int64_t mod) {
    return (a * b) % mod;
}

int64_t Divide(int64_t a, int64_t b, int64_t mod) {
    return Mult(a, InverseMod(b, mod), mod);
}

int64_t Pow(int64_t number, int64_t degree, int64_t mod) {
    if (degree == 0) {
        return 1;
    }
    int64_t multiplier = Pow(number, degree / 2, mod);
    int64_t result = Mult(multiplier, multiplier, mod);
    if (degree % 2 != 0) {
        result = Mult(result, number, mod);
    }
    return result;
}

int64_t Factorial(int64_t number, int64_t mod) {
    int64_t result = 1;
    for (size_t i = 2; i <= number; ++i) {
        result = Mult(result, i, mod);
    }
    return result;
}

int64_t Binomial(int64_t n, int64_t k, int64_t mod) {
    assert(n > 0 && k >= 0 && n >= k);
    return Divide(Factorial(n, mod), Mult(Factorial(k, mod), Factorial(n - k, mod), mod), mod);
}

int64_t BinomialWithRepeats(int64_t n, int64_t k, int64_t mod) {
    return Binomial(n + k - 1, k, mod);
}

int64_t GcdExtended(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    int64_t x1, y1;
    int64_t d = GcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

int64_t InverseMod(int64_t number, int64_t mod) {
    int64_t x, y;
    int64_t gcd = GcdExtended(number, mod, x, y);
    assert(gcd == 1);
    int64_t result = (x % mod + mod) % mod;
    assert(Mult(number, result, mod) == 1);
    return result;
}

template <typename T>
vector<T> FindSimpleDivs(T number) {
    vector<T> divs;
    T i = 2;
    T temp = number;
    while (i * i <= number) {
        while (temp % i == 0) {
            divs.push_back(i);
            temp /= i;
        }
        ++i;
    }
    if (temp != 1) {
        divs.push_back(temp);
    }
    return divs;
}

template <typename T>
unordered_map<T, int> FindSimpleDivsMap(T number) {
    vector<T> divs = FindSimpleDivs(number);
    unordered_map<T, int> divs_map;
    for (T div : divs) {
        ++divs_map[div];
    }
    return divs_map;
}

template <typename T>
T Gcd(T a, T b) {
    return (b == 0) ? a : Gcd(b, a % b);
}