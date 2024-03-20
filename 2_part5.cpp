// g++ -std=c++11 -march=native -O3 2_part5.cpp -o avx512_operations

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




#include <cstdlib>
#include <random>
#include <vector>
#include <immintrin.h>


int main() {
    const size_t n = 10000;  // Vector size
    const int num_iterations = 10000;  // Number of iterations for timing

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Allocate aligned memory for arrays
    double* a_avx = static_cast<double*>(aligned_alloc(64, n * sizeof(double)));
    double* b_avx = static_cast<double*>(aligned_alloc(64, n * sizeof(double)));
    double* c_avx = static_cast<double*>(aligned_alloc(64, n * sizeof(double)));
    double* d_avx = static_cast<double*>(aligned_alloc(64, n * sizeof(double)));
    double* e_avx = static_cast<double*>(aligned_alloc(64, n * sizeof(double)));

    // Fill arrays with random doubles
    for (size_t i = 0; i < n; ++i) {
        b_avx[i] = dis(gen);
        c_avx[i] = dis(gen);
        d_avx[i] = dis(gen);
        e_avx[i] = dis(gen);
    }

    timer t_avx("AVX-512 Operations");
    volatile double sum_avx = 0.0;

    for (int iter = 0; iter < num_iterations; ++iter) {
        for (size_t i = 0; i < n; i += 8) {
            __m512d b = _mm512_loadu_pd(&b_avx[i]);
            __m512d c = _mm512_loadu_pd(&c_avx[i]);
            __m512d d = _mm512_loadu_pd(&d_avx[i]);
            __m512d e = _mm512_loadu_pd(&e_avx[i]);

            __m512d result = _mm512_fmsub_pd(b, c, _mm512_div_pd(d, e));
            result = _mm512_sub_pd(result, _mm512_set1_pd(3.14));

            _mm512_storeu_pd(&a_avx[i], result);
        }
        sum_avx += _mm512_reduce_add_pd(_mm512_loadu_pd(a_avx));  // Force the compiler to consider the loop as having an observable effect
    }

    // Free allocated memory
    free(a_avx);
    free(b_avx);
    free(c_avx);
    free(d_avx);
    free(e_avx);

    return 0;
}