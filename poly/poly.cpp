#include "poly.h"

#include <iostream>

Poly::Poly() {
}

Poly::Poly(const std::vector<std::pair<int64_t, int64_t>>& coefficients) {
    for (auto [k, v] : coefficients) {
        if (v != 0) {
            coefficients_.emplace_back(k, v);
        }
    }
}

Poly::Poly(const std::vector<int64_t>& coefficients) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        if (coefficients[i] != 0) {
            coefficients_.emplace_back(i, coefficients[i]);
        }
    }
}

int64_t Poly::NotThatMuchOfABinPow(int64_t a, int64_t n) const {
    int64_t result = 1;
    for (int i = 0; i < n; ++i) {
        result *= a;
    }
    return result;
}

int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;
    for (size_t i = 0; i < coefficients_.size(); ++i) {
        result += coefficients_[i].second * NotThatMuchOfABinPow(x, coefficients_[i].first);
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    return coefficients_ == other.coefficients_;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*(this) == other);
}

Poly Poly::operator-() const {
    std::vector<std::pair<int64_t, int64_t>> result = coefficients_;
    for (auto& x : result) {
        x.second *= -1;
    }
    return Poly(result);
}

Poly Poly::operator+(const Poly& other) const {
    std::vector<std::pair<int64_t, int64_t>> new_coefficients;
    size_t i = 0;
    size_t j = 0;
    while (i < coefficients_.size() && j < other.coefficients_.size()) {
        if (coefficients_[i].first < coefficients_[j].first) {
            new_coefficients.push_back(coefficients_[i]);
            ++i;
        } else if (coefficients_[i].first == other.coefficients_[j].first) {
            int64_t coefficient = coefficients_[i].second + other.coefficients_[j].second;
            if (coefficient != 0) {
                new_coefficients.emplace_back(coefficients_[i].first, coefficient);
            }
            ++i;
            ++j;
        } else {
            new_coefficients.push_back(other.coefficients_[j]);
            ++j;
        }
    }
    while (i < coefficients_.size()) {
        new_coefficients.push_back(coefficients_[i]);
        ++i;
    }
    while (j < other.coefficients_.size()) {
        new_coefficients.push_back(other.coefficients_[j]);
        ++j;
    }
    return Poly(new_coefficients);
}

Poly Poly::operator-(const Poly& other) const {
    return *(this) + (-other);
}

Poly Poly::operator+=(const Poly& other) {
    std::vector<std::pair<int64_t, int64_t>> new_coefficients;
    size_t i = 0;
    size_t j = 0;
    while (i < coefficients_.size() && j < other.coefficients_.size()) {
        if (coefficients_[i].first < coefficients_[j].first) {
            new_coefficients.push_back(coefficients_[i]);
            ++i;
        } else if (coefficients_[i].first == other.coefficients_[j].first) {
            int64_t coefficient = coefficients_[i].second + other.coefficients_[j].second;
            if (coefficient != 0) {
                new_coefficients.emplace_back(coefficients_[i].first, coefficient);
            }
            ++i;
            ++j;
        } else {
            new_coefficients.push_back(other.coefficients_[j]);
            ++j;
        }
    }
    while (i < coefficients_.size()) {
        new_coefficients.push_back(coefficients_[i]);
        ++i;
    }
    while (j < other.coefficients_.size()) {
        new_coefficients.push_back(other.coefficients_[j]);
        ++j;
    }
    coefficients_ = new_coefficients;
    return *this;
}

Poly Poly::operator-=(const Poly& other) {
    return *(this) += (-other);
}

Poly Poly::operator*(const Poly& other) const {
    std::map<int64_t, int64_t> result;
    for (size_t i = 0; i < coefficients_.size(); ++i) {
        for (size_t j = 0; j < other.coefficients_.size(); ++j) {
            result[coefficients_[i].first + other.coefficients_[j].first] +=
                coefficients_[i].second * coefficients_[j].second;
        }
    }
    std::vector<std::pair<int64_t, int64_t>> result_vector;
    for (auto [k, v] : result) {
        if (v != 0) {
            result_vector.emplace_back(k, v);
        }
    }
    return Poly(result_vector);
}

std::ostream& operator<<(std::ostream& out, const Poly& a) {
    out << "y = ";
    if (a.coefficients_.empty()) {
        out << "0";
        return out;
    }
    for (size_t i = a.coefficients_.size(); i >= 1; --i) {
        out << a.coefficients_[i - 1].second;
        if (a.coefficients_[i - 1].first >= 2) {
            out << "x^" << a.coefficients_[i - 1].first;
        } else if (a.coefficients_[i - 1].first == 1) {
            out << "x";
        }
        if (i != 1) {
            out << " + ";
        }
    }
    return out;
}
