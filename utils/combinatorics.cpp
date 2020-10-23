//
// Created by jugos000 on 23-Oct-20.
//

#include <iomanip>
#include <iostream>

using namespace std;

void variationsWithRepetition(const int n, const int k) {
    int q;
    int number[k];
    for (int i = 0; i < k; i++)
        number[i] = 0;

    do {
        for (int i = 0; i < k; i++)
            cout << setprecision(5) << number[i] + 1 << " ";
        cout << endl;

        q = k - 1;
        while (q >= 0) {
            number[q]++;
            if (number[q] == n) {
                number[q] = 0;
                q--;
            } else
                break;
        }
    } while (q >= 0);
}

void combinationsWithoutRepetition(const int n, const int k) {
    bool b;
    int P[k];

    for (int i = 0; i < k; i++)
        P[i] = i + 1;

    do {
        for (int i = 0; i < k; i++)
            cout << setprecision(3) << P[i] << " ";
        cout << endl;

        b = false;
        for (int i = k - 1; i >= 0; i--) {
            if (P[i] < n - k + 1 + i) {
                P[i]++;
                for (int j = i + 1; j < k; j++)
                    P[j] = P[j - 1] + 1;

                b = true;
                break;
            }
        }
    } while (b);
}

#include "../homework_2/permutations.h"

int main() {
    Permutations p(1, 4);

    do {
        cout << setprecision(5) << p << endl;
        p.Next();
    } while (p.HasNext());
}