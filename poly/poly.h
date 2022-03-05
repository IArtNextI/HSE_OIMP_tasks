#pragma once

#include <iostream>
#include <map>
#include <vector>

class Poly {
private:
    std::vector<std::pair<int64_t, int64_t>> coefficients_;
    int64_t NotThatMuchOfABinPow(int64_t a, int64_t n) const;

public:
    Poly();
    Poly(const std::vector<int64_t>& coefficients);
    Poly(const std::vector<std::pair<int64_t, int64_t>>& coefficients);

    int64_t operator()(int64_t x) const;
    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;
    Poly operator-() const;
    Poly operator-(const Poly& other) const;
    Poly operator+(const Poly& other) const;
    Poly operator+=(const Poly& other);
    Poly operator-=(const Poly& other);
    Poly operator*(const Poly& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Poly& a);
};
