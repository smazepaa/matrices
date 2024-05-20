#include <iostream>
#include <fstream>
#include <thread>
#include <limits>
#include <vector>
#include <chrono>
#include <numeric>

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
vector<int> x (n, 0);

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

void calculateX(){
    // iterate over each pair (x5 and x6, x4 and x7, etc.)
    for (int k = 0; k < n / 2; ++k) {
        int i = n / 2 - (k + 1);
        int j = n / 2 + k;

        int sum_i = y[i];
        int sum_j = y[j];

        for (int l = i; l < j; l++){
            sum_i -= z[i][l] * x[l];
            sum_j -= z[j][n-l-1] * x[n-l-1];
        }

        // z[i][i] * x[i] + z[i][j] * x[j] = sum_i;
        // z[j][i] * x[i] + z[j][j] * x[j] = sum_j;

        int determ = z[j][j] * z[i][i] - z[j][i] * z[i][j];

        int det1 = z[j][j] * sum_i - z[i][j] * sum_j;
        int det2 = z[i][i] * sum_j - z[j][i] * sum_i;

        x[i] = det1/determ;
        x[j] = det2/determ;
    }
}

int main() {

    std::ofstream outfile("execution_times.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return 1;
    }

    // fill w [a][b], where a - row, b - column
    // w is filled by rows (i.e. first rows of column 1, then 2 etc.)
    thread threadZ(fillZ);
    thread threadW(fillW);

    threadZ.join();
    threadW.join();

    std::vector<double> times;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int run = 1; run <= 10; ++run) {

        for (int k = 0; k <= m; ++k) {
            int start = k;
            int end = n - (k + 1);

            calculateZ(start, end);
            if (k != m) {
                calculateW(start, end);
            }
        }

        calculateY();
        calculateX();

    //    cout << "Matrix X:" << endl;
    //    for (int i = 0; i < n; ++i){
    //        cout << x[i] << endl;
    //    }

        // Record the end time
        auto endTime = std::chrono::high_resolution_clock::now();

        // Calculate the duration
        std::chrono::duration<double> duration = endTime - startTime;

        // Store the duration
        times.push_back(duration.count());

        // Write the duration to the file
        outfile << "Run " << run << ": " << duration.count() << " seconds" << std::endl;

    }

    // Calculate and write the average execution time
    double average = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    outfile << "Average execution time: " << average << " seconds" << std::endl;

    // Close the file
    outfile.close();

    std::cout << "Execution times written to execution_times.txt" << std::endl;

    return 0;
}