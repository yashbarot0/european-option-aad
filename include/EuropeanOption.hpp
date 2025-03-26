#ifndef EUROPEAN_OPTION_HPP
#define EUROPEAN_OPTION_HPP

#include <cmath>

class EuropeanOption {
private:
    double S, K, T, r, sigma;
    double d1, d2, N_d1, N_d2, price;

    // Cumulative distribution function for standard normal distribution
    static double norm_cdf(double x);

public:
    EuropeanOption(double S_, double K_, double T_, double r_, double sigma_);
    void calculate();
    void adjoint(double& dS, double& dK, double& dT, double& dr, double& dsigma);
    double getPrice() const;
    double getDelta() const;
    double getGamma() const;
    double getVega() const;
    double getTheta() const;
    double getRho() const;
    
    // Finite differences methods
    static double calculateDeltaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon);
    static double calculateGammaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon);
    static double calculateVegaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon);
    static double calculateThetaFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon);
    static double calculateRhoFiniteDifference(double S, double K, double T, double r, double sigma, double epsilon);
};

#endif // EUROPEAN_OPTION_HPP