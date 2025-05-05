// Practical 3: Implement Min, Max, Sum and Average operations using Parallel Reduction.  

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cstdlib>   // for rand(), srand()
#include <ctime>     // for time()
#include <omp.h>     // for OpenMP

using namespace std;

int random100() {
    return rand() % 100;
}

int findMin(vector<int>& data) {
    int minVal = numeric_limits<int>::max();
    #pragma omp parallel for reduction(min:minVal)
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] < minVal)
            minVal = data[i];
    }
    return minVal;
}

int findMax(vector<int>& data) {
    int maxVal = numeric_limits<int>::min();
    #pragma omp parallel for reduction(max:maxVal)
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] > maxVal)
            maxVal = data[i];
    }
    return maxVal;
}

int findSum(vector<int>& data) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum;
}

double findAverage(vector<int>& data) {
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum / data.size();
}

int main() {
    srand(time(0));  // Seed random number generator

    vector<int> data(100);
    generate(data.begin(), data.end(), random100);

    cout << "Data:" << endl;
    for (int i = 0; i < data.size(); ++i) {
        cout << data[i] << " ";
        if ((i + 1) % 10 == 0) cout << endl;
    }

    cout << "\nMinimum : " << findMin(data) << endl;
    cout << "Maximum : " << findMax(data) << endl;
    cout << "Sum     : " << findSum(data) << endl;
    cout << "Average : " << findAverage(data) << endl;

    return 0;
}