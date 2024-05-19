#include <iostream>
#include <cmath>
#include <thread>
#include <limits>
#include <vector>
//#include <Eigen/Dense>
//
//using namespace Eigen;
using namespace std;

const int n = 10;

vector<vector<int>> z(n, vector<int>(n, INT_MIN));
vector<vector<int>> w(n, vector<int>(n, INT_MIN));

vector<vector<int>> a = {
        {2, 1, 3, 6, 3, 3, 2, 2, 1, 5},
        {10, 10, 17, 33, 18, 15, 15, 13, 12, 12},
        {12, 127, 210, 286, 227, 84, 155, 103, 81, 17},
        {14, 66, 223, 191, 220, 153, 121, 66, 36, 22},
        {18, 61, 233, 195, 269, 167, 153, 75, 44, 19},
        {21, 66, 266, 281, 287, 193, 178, 87, 52, 20},
        {10, 66, 210, 225, 208, 127, 115, 67, 40, 12},
        {8, 125, 206, 273, 223, 104, 149, 91, 72, 7},
        {16, 14, 23, 52, 24, 33, 21, 16, 17, 14},
        {3, 1, 1, 5, 1, 4, 2, 2, 3, 1}
};

vector<int> b = {159, 830, 6487, 5516, 6223, 7278, 5295, 6196, 1207, 124};
vector<int> y (n, INT_MIN);

int m = (n - 1) / 2;
int p = floor((n + 1) / 2.0);
int q = ceil((n + 1) / 2.0);

void fillW() {
    // first formula for w
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else if (j > i && j < (n - i - 1)) {
                w[j][i] = INT_MIN;
            } else {
                w[j][i] = 0;
            }
        }
    }

    // filling for the second formula
    for (int i = p - 1; i < q; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else {
                w[j][i] = 0;
            }
        }
    }

    // filling for the third
    for (int i = q; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                w[j][i] = 1;
            } else if (j >= (n - i) && j < i) {
                w[j][i] = INT_MIN;
            } else {
                w[j][i] = 0;
            }
        }
    }
}

void fillZ() {
    for (int i = 0; i < p; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j >= i && j < (n - i)) {
                z[i][j] = INT_MIN;
            } else {
                z[i][j] = 0;
            }
        }
    }

    for (int i = p; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j <= i && j >= (n - i - 1)) {
                z[i][j] = INT_MIN;
            } else {
                z[i][j] = 0;
            }
        }
    }
}

void calculateZ(int& start, int& end){
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
}

void calculateW(int& start, int& end){
    for (int row = start + 1; row < end; row++){

        int a_start = a[row][start];
        int a_end = a[row][end];

        for (int i = start; i > 0; --i){
            a_start -= w[row][i - 1] * z[i - 1][start];
            a_start -= w[row][n-i] * z[n-i][start];

            a_end -= w[row][i - 1] * z[i - 1][end];
            a_end -= w[row][n-i] * z[n-i][end];
        }

        // w[row][start] * z[start][start] + w[row][end] * z[end][start] = a_start;
        // w[row][start] * z[start][end] + w[row][end] * z[end][end] = a_end;

        int determ = z[end][end] * z[start][start] - z[start][end] * z[end][start];

        int det1 = z[end][end] * a_start - z[end][start] * a_end;
        int det2 = a_end * z[start][start] - a_start * z[start][end];

        w[row][start] = det1/determ;
        w[row][end] = det2/determ;

    }
}

void calculateY(){
    // iterate over each pair (y1 and y10, y2 and y9, etc.)
    for (int i = 0; i < n / 2; ++i) {
        int j = n - i - 1;
        y[i] = b[i];
        y[j] = b[j];

        // the first from the pair
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                y[i] -= w[i][k] * y[k];
            }
        }

        // second
        for (int k = 0; k < n; ++k) {
            if (k != j) {
                y[j] -= w[j][k] * y[k];
            }
        }
    }
}

int main() {

    // fill w [a][b], where a - row, b - column
    // w is filled by rows (i.e. first rows of column 1, then 2 etc.)
    thread threadZ(fillZ);
    thread threadW(fillW);

    threadZ.join();
    threadW.join();

    for (int k = 0; k <= m; ++k){
        int start = k;
        int end = n - (k + 1);

        calculateZ(start, end);
        if (k != m){
            calculateW(start, end);
        }
    }

//    // Вивід результатів
//    cout << "Matrix W:" << endl;
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            cout << w[i][j] << " ";
//        }
//        cout << endl;
//    }
//
//    cout << "Matrix Z:" << endl;
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            cout << z[i][j] << " ";
//        }
//        cout << endl;
//    }

    //// calculating y
    calculateY();

    cout << "Matrix Y:" << endl;
    for (int i = 0; i < n; ++i){
        cout << y[i] << endl;
    }

    return 0;
}
