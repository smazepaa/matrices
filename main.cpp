#include <iostream>
#include "src/Calculator.h"

using namespace std;

const int n = 1000;

int main() {
    Calculator calculator(n, "matrix_data.txt");

    auto start1 = std::chrono::high_resolution_clock::now();
    auto result1 = calculator.solveSequentially();
    auto end1 = std::chrono::high_resolution_clock::now();

    auto dif1 = end1-start1;
    cout << "sequentially " << dif1.count() << endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    auto result2 = calculator.solveConcurrently();
    auto end2 = std::chrono::high_resolution_clock::now();

    auto dif2 = end2-start2;
    cout << "concurrently " << dif2.count() << endl;

//    cout << "Matrix X:" << endl;
//    for (int i = 0; i < n; ++i){
//        cout << result1[i] << endl;
//    }

    return 0;
}
