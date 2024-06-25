#include <iostream>
#include "src/Calculator.h"

using namespace std;

const int n = 1000;

int main() {
    Calculator calculator(n, "matrix_data.txt");

    for (int i = 0; i < 10; i++) {
        cout << "run " << i + 1 << endl;
        auto start1 = std::chrono::high_resolution_clock::now();
        auto result1 = calculator.solveSequentially();
        auto end1 = std::chrono::high_resolution_clock::now();

        auto dif1 = end1 - start1;
        cout << "sequentially " << dif1.count() << endl;

        auto start2 = std::chrono::high_resolution_clock::now();
        auto result2 = calculator.solveConcurrently();
        auto end2 = std::chrono::high_resolution_clock::now();

        auto dif2 = end2 - start2;
        cout << "concurrently " << dif2.count() << endl << endl;

//        cout << "Matrix X:" << endl;
//        for (int j = 0; j < n; ++j){
//            cout << result2[j] << endl;
//        }
    }

//    auto start2 = std::chrono::high_resolution_clock::now();
//    auto result2 = calculator.solveConcurrently();
//    auto end2 = std::chrono::high_resolution_clock::now();
//
//    auto dif2 = end2 - start2;
//    cout << "concurrently " << dif2.count() << endl << endl;
//
//        cout << "Matrix X:" << endl;
//        for (int j = 0; j < n; ++j){
//            cout << result2[j] << endl;
//        }
    return 0;
}
