
// g++ -std=c++11 -O3 -DARMA_DONT_USE_WRAPPER -I eigen-3.4.0/ 2_part4.cpp -o performance_comparison -larmadillo



#include <armadillo>
#include <Eigen/Dense>
#include <valarray>
#include <random>



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

    // Create Armadillo vectors filled with random doubles
    arma::vec b_arma = arma::vec(n).imbue([&]() { return dis(gen); });
    arma::vec c_arma = arma::vec(n).imbue([&]() { return dis(gen); });
    arma::vec d_arma = arma::vec(n).imbue([&]() { return dis(gen); });
    arma::vec e_arma = arma::vec(n).imbue([&]() { return dis(gen); });

    // Create Eigen vectors filled with random doubles
    Eigen::VectorXd b_eigen = Eigen::VectorXd::NullaryExpr(n, [&]() { return dis(gen); });
    Eigen::VectorXd c_eigen = Eigen::VectorXd::NullaryExpr(n, [&]() { return dis(gen); });
    Eigen::VectorXd d_eigen = Eigen::VectorXd::NullaryExpr(n, [&]() { return dis(gen); });
    Eigen::VectorXd e_eigen = Eigen::VectorXd::NullaryExpr(n, [&]() { return dis(gen); });

    // Create std::valarrays filled with random doubles
    std::valarray<double> b_val(n);
    std::valarray<double> c_val(n);
    std::valarray<double> d_val(n);
    std::valarray<double> e_val(n);

    for (size_t i = 0; i < n; ++i) {
        b_val[i] = dis(gen);
        c_val[i] = dis(gen);
        d_val[i] = dis(gen);
        e_val[i] = dis(gen);
    }

    // Perform vector operations using Armadillo
    {
        timer t("Armadillo");
        arma::vec a_arma;

        for (int iter = 0; iter < num_iterations; ++iter) {
            a_arma = b_arma % c_arma + d_arma / e_arma - 3.14;
        }
    }

    // Perform vector operations using Eigen
    {
        timer t("Eigen");
        Eigen::VectorXd a_eigen;

        for (int iter = 0; iter < num_iterations; ++iter) {
            a_eigen = b_eigen.cwiseProduct(c_eigen) + d_eigen.cwiseQuotient(e_eigen) - Eigen::VectorXd::Constant(n, 3.14);
        }
    }

    // Perform vector operations using std::valarray
    {
        timer t("std::valarray");
        std::valarray<double> a_val;

        for (int iter = 0; iter < num_iterations; ++iter) {
            a_val = b_val * c_val + d_val / e_val - 3.14;
        }
    }

    return 0;
}