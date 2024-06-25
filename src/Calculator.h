#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

class Calculator {
public:
    Calculator(int size, const std::string& filename);
    std::vector<double> solveSequentially();
    std::vector<double> solveConcurrently();

private:
    int p, m, n, start, end;
    std::vector<std::vector<double>> z, w, a;
    std::vector<double> b, y, x;

    std::mutex mtx;
    std::condition_variable cv;
    bool z_ready = false;
    bool w_ready = false;

    void fillZ();
    void fillW();
    void readMatrices(const std::string& filename);
    void calculateZ();
    void calculateW();
    void calculateY();
    void calculateX();
    void calculateZconc();
    void calculateWconc();
};
