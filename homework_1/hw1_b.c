#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    const int totalCost = 711;
    const int increment = 1;

    size_t solutionCount = 0;
    size_t numberOfCalls = 0;

    time_t start = clock();

    for (int a = 1; a < totalCost; a += increment) {
        for (int b = 1; b < totalCost; b += increment) {
            for (int c = 1; c < totalCost; c += increment) {
                double d1 = totalCost - (a + b + c);
                double d2 = 711000000 / (a * b * c);

                if (d1 == d2) {
                    long r1 = (a + b + c + d1);
                    long r2 = (a * b * c * d1 / 1000000);

                    if (r1 == r2) {
                        solutionCount++;
                        printf("Solution found (a, b, c, d) = (%.2lf,  %.2lf, %.2lf, %.2lf)\n",
                               (double)a / 100, (double)b / 100, (double)c / 100, d1 / 100);
                    }
                }

                numberOfCalls++;
            }
        }
    }

    time_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Number of optimization function calls: %zu\n", numberOfCalls);
    printf("Time needed for the computation: %.5lf s", cpu_time_used);

    return 0;
}