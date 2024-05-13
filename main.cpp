#include <iostream>
#include <cmath>
#include <thread>

using namespace std;

int z[10][10];
int w[10][10];

int n = 10;
int m = (n-1)/2;
int p = floor((n+1)/2.0);
int q = ceil((n+1)/2.0);

void fillW(){
    // first formula for w
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j){
            if (j == i){
                w[j][i] = 1;
            }
            else if (j > i && j < (n-i-1)){
                w[j][i] = 2;
            }
            else{
                w[j][i] = 0;
            }
        }
    }

    // filling for the second formula
    for (int i = p - 1; i < q; ++i){
        for (int j = 0; j < n; ++j){
            if (j == i){
                w[j][i] = 1;
            }
            else{
                w[j][i] = 0;
            }
        }
    }

    // filling for the third
    for (int i = q; i < n; ++i) {
        for (int j = 0; j < n; ++j){
            if (j == i){
                w[j][i] = 1;
            }
            else if (j >= (n-i) && j < i){
                w[j][i] = 2;
            }
            else{
                w[j][i] = 0;
            }
        }
    }
}

void fillZ(){
    for (int i = 0; i < p; ++i){
        for (int j = 0; j < n; ++j){
            if (j >= i && j < (n-i)){
                z[i][j] = 2;
            }
            else{
                z[i][j] = 0;
            }
        }
    }

    for (int i = p; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (j <= i && j >= (n-i-1)){
                z[i][j] = 2;
            }
            else{
                z[i][j] = 0;
            }
        }
    }
}


int main() {
    int a[10][10] = {
            {2, 1, 3, 6, 3, 3, 2, 2, 1, 5},
            {10, 10, 17, 33, 18, 15, 15, 13, 12,12},
            {12, 127, 210, 286, 227, 84, 155, 103, 81, 17},
            {14, 66, 223, 191, 220, 153, 121, 66, 36, 22},
            {18, 61, 233, 195, 269, 167, 153, 75, 44, 19},
            {21, 66, 266, 281, 287, 193, 178, 87, 52, 20},
            {10, 66, 210, 225, 208, 127, 115, 67, 40, 12},
            {8, 125, 206, 273, 223, 104, 149, 91, 72, 7},
            {16, 14, 23, 52, 24, 33, 21, 16, 17, 14},
            {3, 1, 1, 5, 1, 4, 2, 2, 3, 1}
    };

    int b[10] = {159, 830, 6487, 5516, 6223, 7278, 5295, 6196, 1207, 124};
    int x[10];

    // fill w [a][b], where a - row, b - column
    // w is filled by rows (i.e. first rows of column 1, then 2 etc.)
    thread threadZ (fillZ);
    thread threadW (fillW);

    threadZ.join();
    threadW.join();

    for (const auto& row : z) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
