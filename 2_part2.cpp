
#include <cstdlib>
#include <random>
#include <vector>

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

int main() {
    const size_t n = 10000;  // Vector size
    const int num_iterations = 10000;  // Number of iterations for timing

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Allocate primitive C arrays using malloc
    double* a_c = static_cast<double*>(std::malloc(n * sizeof(double)));
    double* b_c = static_cast<double*>(std::malloc(n * sizeof(double)));
    double* c_c = static_cast<double*>(std::malloc(n * sizeof(double)));
    double* d_c = static_cast<double*>(std::malloc(n * sizeof(double)));
    double* e_c = static_cast<double*>(std::malloc(n * sizeof(double)));

    // Fill C arrays with random doubles
    for (size_t i = 0; i < n; ++i) {
        b_c[i] = dis(gen);
        c_c[i] = dis(gen);
        d_c[i] = dis(gen);
        e_c[i] = dis(gen);
    }

    timer t_c("C Array Operations");
    volatile double sum_c = 0.0;

    for (int iter = 0; iter < num_iterations; ++iter) {
        for (size_t i = 0; i < n; ++i) {
            a_c[i] = b_c[i] * c_c[i] + d_c[i] / e_c[i] - 3.14;
        }
        sum_c += a_c[0];  // Force the compiler to consider the loop as having an observable effect
    }

    // Free allocated memory
    std::free(a_c);
    std::free(b_c);
    std::free(c_c);
    std::free(d_c);
    std::free(e_c);

    // Declare and fill std::vectors with random doubles
    std::vector<double> a_v(n), b_v(n), c_v(n), d_v(n), e_v(n);
    for (size_t i = 0; i < n; ++i) {
        b_v[i] = dis(gen);
        c_v[i] = dis(gen);
        d_v[i] = dis(gen);
        e_v[i] = dis(gen);
    }

    timer t_v("std::vector Operations (single loop)");
    volatile double sum_v = 0.0;

    for (int iter = 0; iter < num_iterations; ++iter) {
        for (size_t i = 0; i < n; ++i) {
            a_v[i] = b_v[i] * c_v[i] + d_v[i] / e_v[i] - 3.14;
        }
        sum_v += a_v[0];  // Force the compiler to consider the loop as having an observable effect
    }

    return 0;
}