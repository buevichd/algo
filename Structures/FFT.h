#pragma once

#include <vector>
#include <complex>
#include <algorithm>
#include <cassert>

// INTERFACE
template <class T>
std::vector<T> MultiplyPolynomials(const std::vector<T>& left, const std::vector<T>& right,
                                   bool cut_unneeded_zeros = true);

// IMPLEMENTATION

template <class T>
class FastFourierTransformation {
private:
    using Complex = std::complex<double>;
    using ComplexVector = std::vector<Complex>;

public:
    explicit FastFourierTransformation(bool cut_unneeded_zeros)
        : cut_unneeded_zeros_(cut_unneeded_zeros) {
    }

    std::vector<T> MultiplyPolynomials(std::vector<T> left, std::vector<T> right) {
        Initialize(left, right);
        AddZerosToNeededSize(&left);
        AddZerosToNeededSize(&right);

        ComplexVector left_values = ComputeValues(left);
        ComplexVector right_values = ComputeValues(right);

        ComplexVector values = MultiplyValues(left_values, right_values);

        return NormalizeResult(ComputeValues(values, true));
    }

private:
    void Initialize(const std::vector<T>& left, const std::vector<T>& right) {
        size_ = GetNearestTwoPow(left.size() + right.size());
        w_ = std::exp(Complex(0, 2 * M_PI / size_));
        w_pow_cache_ = {1.0, w_};
        order_array_ = GetOrderArray();
    }

    size_t GetNearestTwoPow(size_t number) {
        size_t two_pow = 1;
        while (number > two_pow) {
            two_pow *= 2;
        }
        return two_pow;
    }

    std::vector<size_t> GetOrderArray() {
        std::vector<size_t> prev_order_array;
        std::vector<size_t> order_array = {0};
        for (size_t current_size = 2; current_size <= size_; current_size *= 2) {
            prev_order_array = std::move(order_array);
            order_array = std::vector<size_t>(current_size);
            for (size_t i = 0; i < current_size / 2; ++i) {
                order_array[i] = prev_order_array[i] * 2;
                order_array[current_size / 2 + i] = prev_order_array[i] * 2 + 1;
            }
        }
        return order_array;
    }

    void AddZerosToNeededSize(std::vector<T>* vector_ptr) {
        vector_ptr->reserve(size_);
        while (vector_ptr->size() < size_) {
            vector_ptr->push_back(0);
        }
    }

    template <class U>
    ComplexVector ComputeValues(const std::vector<U>& coefficients, bool reverse = false) {
        assert(IsTwoPow(coefficients.size()));

        ComplexVector values = ReorderCoefficients(coefficients);
        for (size_t current_size = 1; current_size < coefficients.size(); current_size *= 2) {
            for (size_t offset = 0; offset < coefficients.size(); offset += 2 * current_size) {
                ButterflyOperation(reverse, offset, current_size, &values);
            }
        }
        return values;
    }

    template <class U>
    ComplexVector ReorderCoefficients(const std::vector<U>& coefficients) {
        assert(coefficients.size() == size_);
        ComplexVector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[order_array_[i]] = Complex(coefficients[i]);
        }
        return result;
    }

    void ButterflyOperation(bool reverse, size_t offset, size_t current_size,
                            ComplexVector* values_ptr) {

        ComplexVector& values = *values_ptr;

        Complex current_pow_w = 1;
        Complex current_w = GetWPow(size_ / current_size / 2);
        if (reverse) {
            current_w = Complex(1.0) / current_w;
        }

        size_t even_offset = offset;
        size_t odd_offset = offset + current_size;

        for (size_t i = 0; i < current_size; ++i) {
            Complex even_value = values[even_offset + i];
            Complex odd_value = values[odd_offset + i];
            values[offset + i] = even_value + current_pow_w * odd_value;
            values[offset + i + current_size] = even_value - current_pow_w * odd_value;
            current_pow_w *= current_w;
        }
    }

    Complex GetWPow(size_t pow) {
        while (w_pow_cache_.size() <= pow) {
            w_pow_cache_.push_back(w_pow_cache_.back() * w_);
        }
        return w_pow_cache_[pow];
    }

    ComplexVector MultiplyValues(const ComplexVector& left, const ComplexVector& right) {
        assert(left.size() == right.size());
        ComplexVector result(left.size());
        for (size_t i = 0; i < left.size(); ++i) {
            result[i] = left[i] * right[i];
        }
        return result;
    }

    std::vector<T> NormalizeResult(const ComplexVector& vector) {
        std::vector<T> result;
        result.reserve(vector.size());
        std::transform(vector.begin(), vector.end(), std::back_inserter(result),
                       [this](const Complex& value) {
                           return static_cast<T>((value / Complex(size_) + Complex(0.5)).real());
                       });
        while (cut_unneeded_zeros_ && result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        return result;
    }

    bool IsTwoPow(size_t number) {
        while (number > 1) {
            if (number % 2 != 0) {
                return false;
            }
            number /= 2;
        }
        return true;
    }

    bool cut_unneeded_zeros_;
    size_t size_ = 0;
    Complex w_;
    ComplexVector w_pow_cache_;
    std::vector<size_t> order_array_;
};

template <class T>
std::vector<T> MultiplyPolynomials(const std::vector<T>& left, const std::vector<T>& right,
                                   bool cut_unneeded_zeros) {
    return FastFourierTransformation<T>(cut_unneeded_zeros).MultiplyPolynomials(left, right);
}
