#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>

namespace my_chrono = std::chrono;

template<typename T>
std::string to_pretty_str(T n) {
    std::stringstream ss;
    ss.precision(3);
    if (n < 1e4)
        ss << n;
    else if (n < 1e6) {
        int thousands = std::floor(n / 1000);
        ss << thousands << "," << std::setw(3) << std::setfill('0') << static_cast<int>(n - 1000 * thousands);
    }
    else if (n < 1e9)
        ss << n / 1.e6 << " million";
    else if (n < 1e12)
        ss << n / 1.e9 << " billion";
    else
        ss << n / 1.e12 << " trillion";
    return ss.str();
}

class timer {
private:
    std::string label;
    my_chrono::high_resolution_clock::time_point start_time;
    my_chrono::high_resolution_clock::time_point stop_time;
    bool running;
    static bool silent;

public:
    explicit timer(const std::string &label = "some function/code") : label(label), running(false) {
        start();
    }

    ~timer() {
        stop();
        print();
    }

    static void silence() {
        silent = true;
    }

    void set_label(std::string l) {
        label = l;
    }

    const timer &print() const {
        if (!silent) {
            auto duration = my_chrono::duration_cast<my_chrono::microseconds>(stop_time - start_time).count();
            std::cout << to_pretty_str(duration) << " microseconds spent in program unit '" << label << "'" << std::endl;
        }
        return *this;
    }

    timer &start() {
        start_time = my_chrono::high_resolution_clock::now();
        running = true;
        return *this;
    }

    timer &stop() {
        if (running) {
            stop_time = my_chrono::high_resolution_clock::now();
            running = false;
        }
        return *this;
    }

    timer &reset() {
        start_time = my_chrono::high_resolution_clock::now();
        stop_time = start_time;
        running = false;
        return *this;
    }

    timer &reset_and_print() {
        print();
        reset();
        return *this;
    }
};

#endif // TIMER_H
bool timer::silent = false;
#include <vector>
#include <random>

// Overloading arithmetic operators for std::vector<double>
std::vector<double> operator+(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

std::vector<double> operator-(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

std::vector<double> operator*(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] * b[i];
    }
    return result;
}

std::vector<double> operator/(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] / b[i];
    }
    return result;
}

#include <vector>
#include <random>

int main() {
    const size_t n = 1000000;  // Vector size
    const int num_iterations = 100;  // Number of iterations for timing

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Declare and fill vectors with random doubles
    std::vector<double> a(n), b(n), c(n), d(n), e(n);
    for (size_t i = 0; i < n; ++i) {
        b[i] = dis(gen);
        c[i] = dis(gen);
        d[i] = dis(gen);
        e[i] = dis(gen);
    }

    // Create a vector filled with the scalar value 3.14
    std::vector<double> scalar(n, 3.14);

    timer t("Vector Operations");

    for (int iter = 0; iter < num_iterations; ++iter) {
        a = b * c + d / e - scalar;
    }

    return 0;
}