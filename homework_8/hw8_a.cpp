//
// Created by jugos000 on 23-Nov-20.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;

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
constexpr int64_t numberOfIndividuals = 2000;
constexpr int numberToIncludeInSelection = 500;
constexpr int maximumNumberOfMutations = 50;

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> dist(0, 1);
uniform_int_distribution<int> distCrossover(0, numberToIncludeInSelection - 1);
uniform_int_distribution<int> distGenes(0, D - 1);
uniform_real_distribution<double> distMutation(0, 1);
uniform_int_distribution<int> distNumberOfMutations(0, maximumNumberOfMutations);
uniform_int_distribution<int> distIndividuals(0, numberOfIndividuals);

inline int64_t costFunction(const bool *vector) {
    int64_t F = 64 * 1024 * 1024;

    for (size_t i = 0; i < D; i++)
        F -= vector[i] * fileSizes[i];

    if (F >= 0)
        return F;
    else
        return maxSize;
}

struct Individual {
    bool genes[D];
    int64_t cost;
};

struct Population {
    Individual individuals[numberOfIndividuals];
};

bool compareIndividuals(Individual *i1, Individual *i2) {
    return (*i1).cost < (*i2).cost;
}

vector<Individual *> selection(Population &population) {
    vector<Individual *> result, final;

    //int64_t min = numeric_limits<int64_t>::max();

    for (int i = 0; i < numberOfIndividuals; i++) {
        result.push_back(&population.individuals[i]);
        /*if (population.individuals[i].cost < min)
            min = population.individuals[i].cost;*/
    }

    sort(result.begin(), result.end(), compareIndividuals);

    for (int i = 0; i < numberToIncludeInSelection; i++)
        final.push_back(result[i]);

    return final;
}

Population crossover(const vector<Individual *> selection) {
    int cnt = 0;
    Population result;

    while (cnt < numberOfIndividuals) {
        // determine which two individuals will interact
        int first = distCrossover(mt);
        int second = first;
        do {
            second = distCrossover(mt);
        } while (first == second);

        // determine crossover point
        int crossoverPoint = distGenes(mt);

        // exchange genetic material
        for (int i = 0; i < crossoverPoint; i++) {
            result.individuals[cnt].genes[i] = (*selection[first]).genes[i];
            result.individuals[cnt + 1].genes[i] = (*selection[second]).genes[i];
        }
        for (int i = crossoverPoint; i < D; i++) {
            result.individuals[cnt].genes[i] = (*selection[second]).genes[i];
            result.individuals[cnt + 1].genes[i] = (*selection[first]).genes[i];
        }

        result.individuals[cnt].cost = costFunction(result.individuals[cnt].genes);
        result.individuals[cnt + 1].cost = costFunction(result.individuals[cnt + 1].genes);

        cnt += 2;
    }

    return result;
}

void mutate(Population &population) {
    const int numberOfMutations = distNumberOfMutations(mt);

    for (int i = 0; i < numberOfMutations; i++) {
        double mutationProbability = distMutation(mt);

        if (mutationProbability < 0.2) {
            int individual = distIndividuals(mt);
            int flipPosition = distGenes(mt);

            population.individuals[individual].genes[flipPosition] = !population.individuals[individual].genes[flipPosition];
            population.individuals[individual].cost = costFunction(population.individuals[individual].genes);
        }
    }
}

void printIndividual(const Individual &individual) {
    for (int i = 0; i < D; i++)
        cout << individual.genes[i];
    cout << endl;
}

int main(int argc, char **argv) {

    for (int t = 0; t < 20; t++) {
        ofstream fileMin("minimum_" + std::to_string(t + 1) + ".txt");

        int64_t globalMinimum = numeric_limits<int64_t>::max();
        Population globalMinimumPopulation;

        // initialize random population
        Population initialPopulation;
        for (int i = 0; i < numberOfIndividuals; i++) {
            for (int j = 0; j < D; j++)
                initialPopulation.individuals[i].genes[j] = dist(mt);

            initialPopulation.individuals[i].cost = costFunction(initialPopulation.individuals[i].genes);
        }
        globalMinimumPopulation = initialPopulation;

        constexpr int numberOfGenerations = 50;
        for (int x = 0; x < numberOfGenerations; x++) {
            const vector<Individual *> selectedIndividuals = selection(initialPopulation);
            Population newPopulation = crossover(selectedIndividuals);
            mutate(newPopulation);

            initialPopulation = newPopulation;

            int64_t currentBest = selection(initialPopulation)[0]->cost;
            if (currentBest < globalMinimum) {
                globalMinimum = currentBest;
                globalMinimumPopulation = initialPopulation;
            }

            fileMin << globalMinimum << endl;
        }

        const Individual &bestIndividual = *selection(globalMinimumPopulation)[0];

        cout << "Best score: " << bestIndividual.cost << endl;
        printIndividual(bestIndividual);
        cout << "------------------------------" << endl;

        fileMin.close();
    }

    return 0;
}