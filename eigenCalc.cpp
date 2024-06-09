#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include <random>
#include <ctime>
#include <fstream>

const int n = 1000;

int main() {
    // Initialize random number generator
    std::srand(std::time(nullptr));
    std::mt19937 gen(std::rand());
    std::uniform_int_distribution<> dist(1, 100);

    // Generate 100x100 matrix 'a' with random values
    Eigen::MatrixXd a(n, n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            a(i, j) = dist(gen);
        }
    }

    // Generate vector 'b' of length 100 with random values
    Eigen::VectorXd b(n);
    for(int i = 0; i < n; ++i) {
        b(i) = dist(gen);
    }

    // Solve for vector 'x' in the equation a * x = b
    Eigen::VectorXd x = a.colPivHouseholderQr().solve(b);

    // Save matrix 'a' and vector 'b' to a file
    std::ofstream outFile("matrix_data.txt");
    if (outFile.is_open()) {
        outFile << a << "\n\n" << b << "\n";
        outFile.close();
    } else {
        std::cerr << "Unable to open file for writing";
        return 1;
    }

    // Output the result
    std::cout << "Matrix a:\n" << a << "\n\n";
    std::cout << "Vector b:\n" << b << "\n\n";
    std::cout << "Solution vector x:\n" << x << "\n\n";

    return 0;
}
