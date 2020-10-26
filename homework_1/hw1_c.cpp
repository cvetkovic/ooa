#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv) {
    const uint64_t totalCost = 711000000;
    const uint64_t increment = 1;

    uint64_t numberOfCalls = 0;

    auto start = high_resolution_clock::now();

    vector<double> factors;

    uint64_t upperSearchLimit = (uint64_t) (sqrt(totalCost) + 1);
    for (uint64_t i = 2; i < upperSearchLimit; i++) {
        if (totalCost % i == 0) {
            double t1 = (double) i / (double) totalCost / 100;
            double t2 = (double) i / 100;

            if (t1 < 7.11)
                factors.push_back(t1);
            if (t2 < 7.11)
                factors.push_back(t2);
        }
    }

    double epsilon = 0.00000001;

    for (uint64_t i = 0; i < factors.size(); i++) {
        for (uint64_t j = 0; j < factors.size(); j++) {
            for (uint64_t k = 0; k < factors.size(); k++) {
                double p1 = factors[i];
                double p2 = factors[j];
                double p3 = factors[k];
                double p4 = 7.11 - p1 - p2 - p3;

                double product = p1 * p2 * p3 * p4;
                double exact = 7.11;

                if (abs((product - exact)) <= epsilon)
                    cout << "Solution found (a, b, c, d) = (" <<
                         setprecision(3) << p1 << ", " <<
                         setprecision(3) << p2 << ", " <<
                         setprecision(3) << p3 << ", " <<
                         setprecision(3) << p4 << ")" << endl;

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
