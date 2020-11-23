//
// Created by jugos000 on 23-Nov-20.
//

#include <cmath>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

constexpr int64_t fileSizes[64] = {173669, 275487, 1197613, 1549805, 502334, 217684, 1796841, 274708,
                                   631252, 148665, 150254, 4784408, 344759, 440109, 4198037, 329673, 28602,
                                   144173, 1461469, 187895, 369313, 959307, 1482335, 2772513, 1313997, 254845,
                                   486167, 2667146, 264004, 297223, 94694, 1757457, 576203, 8577828, 498382,
                                   8478177, 123575, 4062389, 3001419, 196884, 617991, 421056, 3017627, 131936,
                                   1152730, 2676649, 656678, 4519834, 201919, 56080, 2142553, 326263, 8172117,
                                   2304253, 4761871, 205387, 6148422, 414559, 2893305, 2158562, 465972, 304078,
                                   1841018, 1915571}; // in bytes

constexpr int64_t maxSize = 64 * 1024 * 1024; // in bytes
constexpr int64_t D = 64;
constexpr int64_t maxIterations = 100000;

inline int64_t costFunction(bool *vector) {
    int64_t F = 64 * 1024 * 1024;

    for (size_t i = 0; i < D; i++)
        F -= vector[i] * fileSizes[i];

    if (F >= 0)
        return F;
    else
        return maxSize;
}

inline void makeInitialVector(bool *vector, const size_t vectorSize) {
    srand(time(NULL));
    for (size_t i = 0; i < vectorSize; i++)
        vector[i] = rand() % 2;
}

inline void makeHammingPermutation(const int64_t distance, bool *vector, const size_t vectorSize) {
    srand(time(NULL));
    size_t positions[distance];
    positions[0] = rand() % vectorSize;
    for (size_t i = 1; i < distance; i++) {
        while (true) {
            bool condition = true;
            positions[i] = rand() % vectorSize;

            for (size_t j = 0; j < i; j++) {
                if (positions[j] == positions[i]) {
                    condition = false;
                    break;
                }
            }

            if (condition)
                break;
        }
    }

    for (size_t i = 0; i < distance; i++)
        vector[positions[i]] = !vector[positions[i]];
}

inline void printVector(bool *vector, const size_t vectorSize) {
    for (int i = 0; i < vectorSize; i++)
        std::cout << vector[i];
    std::cout << std::endl;
}

inline void copyVector(bool *src, bool *dst, const size_t vectorSize) {
    for (size_t i = 0; i < vectorSize; i++)
        dst[i] = src[i];
}

int main(int argc, char **argv) {
    std::vector<double> vectorMinimums;
    std::ofstream averages("average.txt");
    std::ofstream minimums("minimum.txt");

    for (int x = 0; x < 20; x++) {
        int64_t iteration = 0;
        int64_t fMin = std::numeric_limits<int64_t>::max();

        constexpr int64_t initialTemperature = 32 * 1024 * 1024;
        bool vector[D];
        bool solution[D];
        bool minimum[D];

        makeInitialVector(vector, D);
        int64_t cost = costFunction(vector);
        copyVector(vector, solution, D);
        copyVector(vector, minimum, D);
        //printVector(vector, D);

        constexpr int64_t h_max = 64;
        constexpr int64_t h_min = 1;
        int64_t T = initialTemperature;

        while (iteration < maxIterations) {
            double tmp1 = (double) (h_min - h_max) / (maxIterations - 1.0);
            double tmp2 = iteration * tmp1;

            int64_t hammingDistance = h_max + tmp2;
            makeHammingPermutation(hammingDistance, vector, D);
            //printVector(vector, D);

            int64_t newCost = costFunction(vector);
            int64_t dE = newCost - cost;
            if (dE < 0) {
                cost = newCost;
                copyVector(vector, solution, D);
            } else {
                double p = (dE == 0 ? 0.5 : exp(-1.0 * dE / T));
                double rnd = (double) (rand() / (RAND_MAX));

                if (rnd < p) {
                    cost = newCost;
                    copyVector(vector, solution, D);
                }
            }

            constexpr double alpha = 0.95;
            T *= alpha;

            if (cost < fMin) {
                fMin = cost;
                copyVector(solution, minimum, D);
            }

            iteration++;
        }

        double unoccupiedSize = ((double) costFunction(minimum)) / (1024 * 1024);
        double occupiedSize = ((double) maxSize / (1024 * 1024)) - unoccupiedSize; // MB
        std::cout << "Occupied size: " << occupiedSize << " MB" << std::endl;
        std::cout << "Unoccupied size: " << unoccupiedSize << " MB" << std::endl;
        std::cout << "-----------------------------" << std::endl;

        vectorMinimums.push_back(unoccupiedSize);

        minimums << unoccupiedSize << std::endl;
        double avg = 0;
        for (int i = 0; i < vectorMinimums.size(); i++) {
            avg += vectorMinimums[i];
        }
        avg /= vectorMinimums.size();
        averages << avg << std::endl;
    }

    double avg = 0;
    double cumulative_minimum = std::numeric_limits<double>::max();
    for (int i = 0; i < 20; i++) {
        avg += vectorMinimums[i];
        if (vectorMinimums[i] < cumulative_minimum)
            cumulative_minimum = vectorMinimums[i];
    }
    avg /= 20;

    std::cout << "Average unoccupied size: " << avg << " MB" << std::endl;
    std::cout << "Cumulative unoccupied size minimum: " << cumulative_minimum << " MB" << std::endl;

    averages.close();
    minimums.close();

    return 0;
}