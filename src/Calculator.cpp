#include "Calculator.h"
#include <fstream>
#include <thread>
#include <limits>

Calculator::Calculator(int size, const std::string& filename) : n(size) {
    p = floor((n + 1) / 2.0);
    m = (n - 1) / 2;

    z = std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::lowest()));
    w = std::vector<std::vector<double>>(n, std::vector<double>(n, std::numeric_limits<double>::lowest()));
    y = std::vector<double>(n, std::numeric_limits<double>::lowest());
    x = std::vector<double>(n, 0);
    b = std::vector<double>(n, 0);
    a = std::vector<std::vector<double>>(n, std::vector<double>(n, 0));

    readMatrices(filename);

    std::thread threadZ(&Calculator::fillZ, this);
    std::thread threadW(&Calculator::fillW, this);

    threadZ.join();
    threadW.join();

}

void Calculator::fillZ() {
    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j >= i && j < (n - i)) {
                z[i][j] = std::numeric_limits<double>::lowest();
            } else {
                z[i][j] = 0;
            }
        }
    }

    for (int i = p; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j <= i && j >= (n - i - 1)) {
                z[i][j] = std::numeric_limits<double>::lowest();
            } else {
                z[i][j] = 0;
            }
        }
    }
}

void Calculator::fillW() {
    int q = ceil((n + 1) / 2.0);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else if (j > i && j < (n - i - 1)) {
                w[j][i] = std::numeric_limits<double>::lowest();
            } else {
                w[j][i] = 0;
            }
        }
    }

    for (int i = p - 1; i < q; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else {
                w[j][i] = 0;
            }
        }
    }

    for (int i = q; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else if (j >= (n - i) && j < i) {
                w[j][i] = std::numeric_limits<double>::lowest();
            } else {
                w[j][i] = 0;
            }
        }
    }
}

void Calculator::readMatrices(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Unable to open file for reading" << std::endl;
        return;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inFile >> a[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        inFile >> b[i];
    }
    inFile.close();
}

void Calculator::calculateZ(){
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = start; i <= end; ++i){
        z[start][i] = a[start][i];
        z[end][i] = a[end][i];

        for (int j = start; j > 0; --j){
            z[start][i] -= w[start][j - 1] * z[j - 1][i];
            z[start][i] -= w[start][n-j] * z[n - j][i];

            z[end][i] -= w[end][j - 1] * z[j - 1][i];
            z[end][i] -= w[end][n-j] * z[n - j][i];
        }
    }
    z_ready = true;
    cv.notify_one();  // Notify that z is ready
}

void Calculator::calculateW(){
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return z_ready; }); // Wait until z is ready

    for (int row = start + 1; row < end; row++){
        double a_start = a[row][start];
        double a_end = a[row][end];

        for (int i = start; i > 0; --i){
            a_start -= w[row][i - 1] * z[i - 1][start];
            a_start -= w[row][n-i] * z[n-i][start];
            a_end -= w[row][i - 1] * z[i - 1][end];
            a_end -= w[row][n-i] * z[n-i][end];
        }

        double determ = z[end][end] * z[start][start] - z[start][end] * z[end][start];

        double det1 = z[end][end] * a_start - z[end][start] * a_end;
        double det2 = a_end * z[start][start] - a_start * z[start][end];

        w[row][start] = det1/determ;
        w[row][end] = det2/determ;
    }
    z_ready = false;  // Reset the flag
}

void Calculator::calculateY(){
    for (int i = 0; i < n / 2; ++i) {
        int j = n - i - 1;
        y[i] = b[i];
        y[j] = b[j];

        for (int k = 0; k < n; ++k) {
            if (k != i) {
                y[i] -= w[i][k] * y[k];
            }
        }

        for (int k = 0; k < n; ++k) {
            if (k != j) {
                y[j] -= w[j][k] * y[k];
            }
        }
    }
}

void Calculator::calculateX() {
    for (int k = 0; k < n / 2; ++k) {
        int i = n / 2 - (k + 1);
        int j = n / 2 + k;

        double sum_i = y[i];
        double sum_j = y[j];

        for (int l = i; l < j; l++){
            sum_i -= z[i][l] * x[l];
            sum_j -= z[j][n-l-1] * x[n-l-1];
        }

        double determ = z[j][j] * z[i][i] - z[j][i] * z[i][j];

        double det1 = z[j][j] * sum_i - z[i][j] * sum_j;
        double det2 = z[i][i] * sum_j - z[j][i] * sum_i;

        x[i] = det1/determ;
        x[j] = det2/determ;
    }
}

std::vector<double> Calculator::solveConcurrently() {
    for (int k = 0; k <= m; ++k) {
        start = k;
        end = n - (k + 1);

        std::thread zThread(&Calculator::calculateZ, this);
        if (k != m) {
            std::thread wThread(&Calculator::calculateW, this);
            wThread.join();
        }
        zThread.join();

    }

    calculateY();
    calculateX();

    return x;
}

std::vector<double> Calculator::solveSequentially() {
    for (int k = 0; k <= m; ++k) {
        start = k;
        end = n - (k + 1);

        calculateZ();
        if (k != m) {
            calculateW();
        }
    }

    calculateY();
    calculateX();

    return x;
}