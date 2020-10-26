#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv) {
    const uint64_t totalCost = 711;
    const uint64_t increment = 1;

    uint64_t solutionCount = 0;
    uint64_t numberOfCalls = 0;

    auto start = high_resolution_clock::now();

#pragma omp parallel for collapse(3) reduction(+: solutionCount, numberOfCalls)
    for (int a = 1; a < totalCost; a += increment) {
        for (int b = 1; b < totalCost; b += increment) {
            for (int c = 1; c < totalCost; c += increment) {
                double d1 = totalCost - (a + b + c);
                double d2 = 711000000 / (a * b * c);

                if (d1 == d2) {
                    uint64_t r1 = (a + b + c + d1);
                    uint64_t r2 = (a * b * c * d1 / 1000000);

                    if (r1 == r2) {
                        solutionCount++;
                        cout << "Solution found (a, b, c, d) = (" <<
                             setprecision(2) << (double) a / 100 << ", " <<
                             setprecision(2) << (double) b / 100 << ", " <<
                             setprecision(2) << (double) c / 100 << ", " <<
                             setprecision(2) << (double) d1 / 100 << ")" << endl;
                    }
                }

                numberOfCalls++;
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Number of optimization function calls: " << numberOfCalls << endl;
    cout << "Time needed for the computation: " << duration.count() << " ms";

    return 0;
}
