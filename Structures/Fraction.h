#pragma once

#include <initializer_list>

template <class T>
T Gcd(T left, T right) {
    return (left == 0) ? right : Gcd(right % left, left);
}

template <class T>
class Fraction {
public:
    Fraction(T nominator = 0, T denominator = 1)
        : nominator_(nominator), denominator_(denominator) {

        Normalize();
    }

    Fraction(const std::initializer_list<T>& initializer_list)
        : Fraction(*initializer_list.begin(), *std::next(initializer_list.begin())) {

        assert(initializer_list.size() == 2);
    }

    T GetNominator() const {
        return nominator_;
    }

    T GetDenominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        assert(denominator_ != 0);
        if (denominator_ < 0) {
            denominator_ = -denominator_;
            nominator_ = -nominator_;
        }
        T gcd = Gcd(std::abs(nominator_), denominator_);
        nominator_ /= gcd;
        denominator_ /= gcd;
    }

    T nominator_;
    T denominator_;
};

template <class T>
Fraction<T> operator+(const Fraction<T>& left, const Fraction<T>& right) {
    T denominator_gcd = Gcd(left.GetDenominator(), right.GetDenominator());
    T common_denominator = left.GetDenominator() * right.GetDenominator() / denominator_gcd;
    T new_left_nominator = left.GetNominator() * common_denominator / left.GetDenominator();
    T new_right_nominator = right.GetNominator() * common_denominator / right.GetDenominator();
    return {new_left_nominator + new_right_nominator, common_denominator};
}

template <class T>
Fraction<T> operator-(const Fraction<T>& fraction) {
    return {-fraction.GetNominator(), fraction.GetDenominator()};
}

template <class T>
Fraction<T> operator-(const Fraction<T>& left, const Fraction<T>& right) {
    return left + (-right);
}

template <class T>
Fraction<T> operator*(const Fraction<T>& left, const Fraction<T>& right) {
    return {left.GetNominator() * right.GetNominator(),
            left.GetDenominator() * right.GetDenominator()};
}


template <class T>
Fraction<T> operator/(const Fraction<T>& left, const Fraction<T>& right) {
    return {left.GetNominator() * right.GetDenominator(),
            left.GetDenominator() * right.GetNominator()};
}

template <class T>
Fraction<T> operator+=(Fraction<T>& left, const Fraction<T>& right) {
    return left = left + right;
}

template <class T>
Fraction<T> operator-=(Fraction<T>& left, const Fraction<T>& right) {
    return left = left - right;
}

template <class T>
Fraction<T> operator*=(Fraction<T>& left, const Fraction<T>& right) {
    return left = left * right;
}

template <class T>
Fraction<T> operator/=(Fraction<T>& left, const Fraction<T>& right) {
    return left = left / right;
}

template <class T>
bool operator==(const Fraction<T>& left, const Fraction<T>& right) {
    return left.GetNominator() == right.GetNominator() &&
        left.GetDenominator() == right.GetDenominator();
}

template <class T>
bool operator!=(const Fraction<T>& left, const Fraction<T>& right) {
    return !(left == right);
}

template <class T>
bool operator<(const Fraction<T>& left, const Fraction<T>& right) {
    return (left - right).GetNominator() < 0;
}

template <class T>
bool operator>(const Fraction<T>& left, const Fraction<T>& right) {
    return right < left;
}

template <class T>
bool operator<=(const Fraction<T>& left, const Fraction<T>& right) {
    return left < right || left == right;
}

template <class T>
bool operator>=(const Fraction<T>& left, const Fraction<T>& right) {
    return right <= left;
}

template <class T>
struct std::hash<Fraction<T>> {
public:
    size_t operator()(const Fraction<T>& fraction) const {
        return std::hash<double>()(static_cast<double>(fraction.GetNominator()) /
                                   fraction.GetDenominator());
    }
};
