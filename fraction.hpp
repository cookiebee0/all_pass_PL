#pragma once
#include <iostream>
#include <numeric> // for gcd in C++17
using namespace std;

class Fraction {
public:
    int num; // 分子
    int den; // 分母

    Fraction(int numerator = 0, int denominator = 1) {
        if (denominator == 0) throw runtime_error("Denominator cannot be zero.");
        int g = gcd(numerator, denominator);
        num = numerator / g;
        den = denominator / g;
        if (den < 0) {
            num = -num;
            den = -den;
        }
    }

    Fraction operator+(const Fraction& other) const {
        return Fraction(num * other.den + other.num * den, den * other.den);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(num * other.den - other.num * den, den * other.den);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(num * other.num, den * other.den);
    }

    Fraction operator/(const Fraction& other) const {
        if (other.num == 0) throw runtime_error("Division by zero.");
        return Fraction(num * other.den, den * other.num);
    }

    bool operator>(const Fraction& other) const {
        return num * other.den > other.num * den;
    }

    bool operator<(const Fraction& other) const {
        return num * other.den < other.num * den;
    }

    bool operator==(const Fraction& other) const {
        return num == other.num && den == other.den;
    }

    friend ostream& operator<<(ostream& os, const Fraction& f) {
        if (f.den == 1) os << f.num;
        else os << f.num << "/" << f.den;
        return os;
    }
};
