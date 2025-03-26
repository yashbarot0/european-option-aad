#include "EuropeanOption.hpp"
#include <cmath>
#include <iostream>
#include <limits>

double EuropeanOption::norm_cdf(double x) {
    double k = 1.0 / (1.0 + 0.2316419 * std::abs(x));
    double k_sum = k * (0.319381530 + k * (-0.356563782 + k * (1.781477937 + k * (-1.821255978 + k * 1.330274429))));
    double pdf = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * x * x);
    return std::erfc(x * std::signbit(x) * M_SQRT1_2) - pdf * k_sum;
}

EuropeanOption::EuropeanOption(double S_, double K_, double T_, double r_, double sigma_)
    : S(S_), K(K_), T(T_), r(r_), sigma(sigma_) {}

void EuropeanOption::calculate() {
    if (S <= 0 || K <= 0 || T <= 0 || sigma <= 0) {
        price = 0;
        d1 = d2 = N_d1 = N_d2 = 0;
        return;
    }

    double ln_term = std::log(S / K);
    double sigma_sqrt_T = sigma * std::sqrt(T);
    d1 = (ln_term + (r + 0.5 * sigma * sigma) * T) / sigma_sqrt_T;
    d2 = d1 - sigma_sqrt_T;
    
    N_d1 = norm_cdf(d1);
    N_d2 = norm_cdf(d2);
    
    price = S * N_d1 - K * std::exp(-r * T) * N_d2;
}

void EuropeanOption::adjoint(double& dS, double& dK, double& dT, double& dr, double& dsigma) {
    if (S <= 0 || K <= 0 || T <= 0 || sigma <= 0) {
        dS = dK = dT = dr = dsigma = 0;
        return;
    }

    double adj_price = 1.0;
    
    // Adjoint for N_d1
    double adj_N_d1 = S * adj_price;
    
    // Adjoint for d1
    double pdf_d1 = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
    double adj_d1 = adj_N_d1 * pdf_d1;
    
    // Adjoint for sigma
    dsigma += adj_d1 * (-d1 / sigma + 0.5 * std::sqrt(T));
    
    // Adjoint for T
    dT += adj_d1 * (-d1 / (2 * sigma * std::sqrt(T)) + 0.5 / std::sqrt(T));
    
    // Adjoint for S
    dS += adj_d1 * (1 / (sigma * std::sqrt(T)) + N_d1);
    
    // Adjoint for K
    dK += adj_d1 * (-1 / (sigma * std::sqrt(T) * K) - std::exp(-r * T) * N_d2);
    
    // Adjoint for r
    dr += adj_price * (-K * T * std::exp(-r * T) * N_d2);
}

double EuropeanOption::getPrice() const {
    return price;
}

double EuropeanOption::getDelta() const {
    return N_d1;
}

double EuropeanOption::getGamma() const {
    double pdf = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
    return pdf / (S * sigma * std::sqrt(T));
}

double EuropeanOption::getVega() const {
    double pdf = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
    return S * std::sqrt(T) * pdf;
}

double EuropeanOption::getTheta() const {
    double pdf = (1.0 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * d1 * d1);
    double theta = (-S * sigma * pdf) / (2 * std::sqrt(T)) - r * K * std::exp(-r * T) * N_d2;
    return theta;
}

double EuropeanOption::getRho() const {
    return K * T * std::exp(-r * T) * N_d2;
}

// Finite differences implementations with error checking
double EuropeanOption::calculateDeltaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon) {
    try {
        EuropeanOption option_plus(S + epsilon, K, T, r, sigma);
        option_plus.calculate();
        double price_plus = option_plus.getPrice();
        
        EuropeanOption option_minus(S - epsilon, K, T, r, sigma);
        option_minus.calculate();
        double price_minus = option_minus.getPrice();
        
        return (price_plus - price_minus) / (2 * epsilon);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double EuropeanOption::calculateGammaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon) {
    try {
        double delta_plus = calculateDeltaFiniteDifference(S + epsilon, K, T, r, sigma, epsilon);
        double delta_minus = calculateDeltaFiniteDifference(S - epsilon, K, T, r, sigma, epsilon);
        return (delta_plus - delta_minus) / (2 * epsilon);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double EuropeanOption::calculateVegaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon) {
    try {
        EuropeanOption option_plus(S, K, T, r, sigma + epsilon);
        option_plus.calculate();
        double price_plus = option_plus.getPrice();
        
        EuropeanOption option_minus(S, K, T, r, sigma - epsilon);
        option_minus.calculate();
        double price_minus = option_minus.getPrice();
        
        return (price_plus - price_minus) / (2 * epsilon);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double EuropeanOption::calculateThetaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon) {
    try {
        EuropeanOption option_plus(S, K, T + epsilon, r, sigma);
        option_plus.calculate();
        double price_plus = option_plus.getPrice();
        
        EuropeanOption option_minus(S, K, T - epsilon, r, sigma);
        option_minus.calculate();
        double price_minus = option_minus.getPrice();
        
        return (price_plus - price_minus) / (2 * epsilon);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double EuropeanOption::calculateRhoFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon) {
    try {
        EuropeanOption option_plus(S, K, T, r + epsilon, sigma);
        option_plus.calculate();
        double price_plus = option_plus.getPrice();
        
        EuropeanOption option_minus(S, K, T, r - epsilon, sigma);
        option_minus.calculate();
        double price_minus = option_minus.getPrice();
        
        return (price_plus - price_minus) / (2 * epsilon);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}