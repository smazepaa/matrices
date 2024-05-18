#include <iostream>
#include <cmath>
#include <thread>
#include <limits>
#include <vector>

using namespace std;

const int n = 10;

vector<vector<int>> z(n, vector<int>(n, INT_MIN));
vector<vector<int>> w(n, vector<int>(n, INT_MIN));

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

int main() {
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

    // fill w [a][b], where a - row, b - column
    // w is filled by rows (i.e. first rows of column 1, then 2 etc.)
    thread threadZ(fillZ);
    thread threadW(fillW);

    threadZ.join();
    threadW.join();

    int k = 0;
    int start = k;
    int end = n - (k + 1);

    for (int i = 0; i < n; ++i){
        z[start][i] = a[start][i];
        z[end][i] = a[end][i];
    }

//    // Вивід результатів
//    cout << "Matrix Z:" << endl;
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            if (z[i][j] == INT_MIN) {
//                cout << "INF ";
//            } else {
//                cout << z[i][j] << " ";
//            }
//        }
//        cout << endl;
//    }

    start++; end--;

    int i = 0; // (i = k)
    for (int row = start; row <= end; row++){

        int determ = z[k][i] * z[9][9] - z[k][9] * z[9][i];
        cout << row << "determ - " << determ << endl;
        if (determ == 0) {
            continue;
        }

        // w[row][i] * z[i][k] + w[row][9] * z[9][k] = a[row][k];
        // w[row][i] * z[i][9] + w[row][9] * z[9][9] = a[row][9];

        int deti = a[row][k] * z[9][9] - a[row][9] * z[9][k];
        cout << row << "deti - " << deti << endl;

        int det9 = a[row][9] * z[k][i] - z[k][9] * a[row][k];
        cout << row << "det9 - " << det9 << endl;

        w[row][i] = deti / determ;
        w[row][9] = det9 / determ;
    }

    // Вивід результатів
    cout << "Matrix W:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (w[i][j] == INT_MIN) {
                cout << "INF ";
            } else {
                cout << w[i][j] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
