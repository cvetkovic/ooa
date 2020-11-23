//
// Created by jugos000 on 16-Nov-20.
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

constexpr double EPSILON = 1e-6;

bool stopCondition(double matrix[][7]) {
    return matrix[0][1] >= 0 && matrix[0][2] >= 0;
}

void printMatrix(double matrix[][7]) {
    cout << endl;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 7; j++) {
            cout << fixed << setprecision(4) << matrix[i][j] << " ";
        }

        cout << endl;
    }

    cout << "---------------------" << endl;
}

int main(int argc, char **argv) {
    double matrix[4][7] = {{1, -20, -30, 0, 0, 0, 0},
                           {0, 1,   3,   1, 0, 0, 75},
                           {0, 3,   1,   0, 1, 0, 99},
                           {0, 1,   6,   0, 0, 1, 288}};

    int pivotRow = -1;
    int currentColumn = 1;

    while (!stopCondition(matrix)) {
        float min = numeric_limits<double>().max();

        // determine pivot row
        for (int i = 1; i <= 3; i++) {
            if (matrix[i][currentColumn] < 0)
                continue;

            double division = matrix[i][6] / matrix[i][currentColumn];
            if (division < min) {
                pivotRow = i;
                min = division;
            }
        }

        // subtract all rows
        for (int i = 0; i <= 3; i++) {
            if (abs(matrix[i][currentColumn]) >= 0 && abs(matrix[i][currentColumn]) <= EPSILON)
                continue;
            else if (i == pivotRow)
                continue;

            double divisor = -matrix[i][currentColumn] / matrix[pivotRow][currentColumn];
            for (int j = 0; j < 7; j++) {
                matrix[i][j] = matrix[i][j] + matrix[pivotRow][j] * divisor;
            }
        }

        currentColumn++;
    }

    printMatrix(matrix);

    double a = matrix[2][6] / matrix[2][1];
    double b = matrix[1][6] / matrix[1][2];

    cout << "a = " << setprecision(4) << a << endl;
    cout << "b = " << setprecision(4) << b << endl;
    cout << "Cost function: " << setprecision(4) << matrix[0][6] << endl;

    long la = (long) a;
    long lb = (long) b;
    const int interval = 5;

    long la_upper = la + interval;
    long lb_upper = lb + interval;

    long lcost = numeric_limits<long>.min();

    for (long i = la - interval; i < la_upper; i++) {
        for (long j = lb - interval; j < lb_upper; j++) {
            long newCost = 20 * i + 30 * j;

            if ((i + 3 * j > 75) || (3 * i + j > 99) || (i + 6 * j > 288))
                continue;

            if (newCost >= lcost) {
                la = i;
                lb = j;
                lcost = newCost;
            }
        }
    }

    cout << "---------------------" << endl;

    cout << "Optimal a: " << la << endl;
    cout << "Optimal b: " << lb << endl;
    cout << "Optimal cost function value: " << lcost << endl;

    return 0;
}