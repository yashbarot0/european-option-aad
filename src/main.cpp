#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "../include/EuropeanOption.hpp"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " S K T r sigma" << std::endl;
        return 1;
    }

    // Read parameters from command line
    double S = std::stod(argv[1]);
    double K = std::stod(argv[2]);
    double T = std::stod(argv[3]);
    double r = std::stod(argv[4]);
    double sigma = std::stod(argv[5]);

    // Create EuropeanOption object
    EuropeanOption option(S, K, T, r, sigma);

    // Calculate price and Greeks using AAD
    auto start_aad = std::chrono::high_resolution_clock::now();
    option.calculate();
    auto end_aad = std::chrono::high_resolution_clock::now();
    double time_aad = std::chrono::duration_cast<std::chrono::microseconds>(end_aad - start_aad).count();

    // Get AAD results
    double price_aad = option.getPrice();
    double delta_aad = option.getDelta();
    double gamma_aad = option.getGamma();
    double vega_aad = option.getVega();
    double theta_aad = option.getTheta();
    double rho_aad = option.getRho();

    // Calculate Greeks using finite differences
    double epsilon = 0.01;
    auto start_fd = std::chrono::high_resolution_clock::now();
    double delta_fd = EuropeanOption::calculateDeltaFiniteDifference(S, K, T, r, sigma, epsilon);
    double gamma_fd = EuropeanOption::calculateGammaFiniteDifference(S, K, T, r, sigma, epsilon);
    double vega_fd = EuropeanOption::calculateVegaFiniteDifference(S, K, T, r, sigma, epsilon);
    double theta_fd = EuropeanOption::calculateThetaFiniteDifference(S, K, T, r, sigma, epsilon);
    double rho_fd = EuropeanOption::calculateRhoFiniteDifference(S, K, T, r, sigma, epsilon);
    auto end_fd = std::chrono::high_resolution_clock::now();
    double time_fd = std::chrono::duration_cast<std::chrono::microseconds>(end_fd - start_fd).count();

    // Write results to standard output
    std::cout << "Price (AAD): " << price_aad << std::endl;
    std::cout << "Delta (AAD): " << delta_aad << std::endl;
    std::cout << "Gamma (AAD): " << gamma_aad << std::endl;
    std::cout << "Vega (AAD): " << vega_aad << std::endl;
    std::cout << "Theta (AAD): " << theta_aad << std::endl;
    std::cout << "Rho (AAD): " << rho_aad << std::endl;

    std::cout << "\nFinite Differences Results (epsilon = " << epsilon << "):\n";
    std::cout << "Delta (FD): " << delta_fd << std::endl;
    std::cout << "Gamma (FD): " << gamma_fd << std::endl;
    std::cout << "Vega (FD): " << vega_fd << std::endl;
    std::cout << "Theta (FD): " << theta_fd << std::endl;
    std::cout << "Rho (FD): " << rho_fd << std::endl;

    std::cout << "\nExecution Times:\n";
    std::cout << "AAD Calculation Time: " << time_aad << " microseconds" << std::endl;
    std::cout << "Finite Differences Calculation Time: " << time_fd << " microseconds" << std::endl;

    return 0;
}