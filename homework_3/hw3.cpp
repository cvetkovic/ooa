//
// Created by jugos000 on 26-Oct-20.
//

#ifndef HOMEWORK_3_H
#define HOMEWORK_3_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

#include "edge.hpp"
#include "variations.hpp"

using namespace std;
using namespace std::chrono;

const uint32_t numberOfCities = 10;
uint64_t costTable[numberOfCities][numberOfCities] = {{0,   374, 200, 223, 108, 178, 252, 285, 240, 356},
                                                      {374, 0,   255, 166, 433, 199, 135, 95,  136, 17},
                                                      {200, 255, 0,   128, 277, 821, 180, 160, 131, 247},
                                                      {223, 166, 128, 0,   430, 47,  52,  84,  40,  155},
                                                      {108, 433, 277, 430, 0,   453, 478, 344, 389, 423},
                                                      {178, 199, 821, 47,  453, 0,   91,  110, 64,  181},
                                                      {252, 135, 180, 52,  478, 91,  0,   114, 83,  117},
                                                      {285, 95,  160, 84,  344, 110, 114, 0,   47,  78},
                                                      {240, 136, 131, 40,  389, 64,  83,  47,  0,   118},
                                                      {356, 17,  247, 155, 423, 181, 117, 78,  118, 0}};

inline uint64_t CostFunction(vector<SpanningTreeEdge> &edges, unordered_map<uint64_t, uint64_t> &gMatrix) {
    uint64_t result = 0;

    for (uint64_t i = 0; i < edges.size(); i++)
        result += costTable[edges[i].from][edges[i].to];

    for (uint64_t i = 0; i < gMatrix.size(); i++)
        if (gMatrix[i] >= 4)
            result += 100 * (gMatrix[i] - 3);

    return result;
}

/**
 *
 * @param pVector
 * @param pLength
 * @param vVector Sorted vector of all vertices indexes
 * @return
 */
vector<SpanningTreeEdge> *VariationToSpanningTree(vector<uint64_t> &pVector, vector<uint64_t> &vVector,
                                                  unordered_map<uint64_t, uint64_t> &gVector) {
    auto *edges = new vector<SpanningTreeEdge>();

    while (!pVector.empty()) {
        for (uint64_t i = 0; i < vVector.size(); i++) {
            if (find(pVector.begin(), pVector.end(), vVector[i]) == pVector.end()) {
                edges->push_back(SpanningTreeEdge(pVector[0], vVector[i]));
                gVector[pVector[0]]++;
                pVector.erase(pVector.begin());
                vVector.erase(vVector.begin() + i);
                break;
            }
        }
    }

    edges->push_back(SpanningTreeEdge(vVector[0], vVector[1]));
    gVector[vVector[0]]++;

    return edges;
}

int main(int argc, char **argv) {
    // delete later
    const int n = 2;
    const int k = n - 2;

    VariationGenerator generator(n, k);
    uint64_t *resultVector = new uint64_t[k];

    vector<SpanningTreeEdge> *minimumEdges = nullptr;
    uint64_t minimumScore = numeric_limits<uint64_t>::max();

    auto start = high_resolution_clock::now();

    uint64_t iteration = 0;

    while (generator.HasNext()) {
        iteration++;
        generator.NextVariation(resultVector);
        vector<uint64_t> pVector, vVector;
        unordered_map<uint64_t, uint64_t> gVector;

        for (int i = 0; i < k; i++)
            pVector.push_back(resultVector[i]);

        for (uint64_t i = 0; i < n; i++) {
            vVector.push_back(i);
            gVector[i] = 0;
        }

        vector<SpanningTreeEdge> *edges = VariationToSpanningTree(pVector, vVector, gVector);

#ifdef DEBUG
        for (int i = 0; i < edges->size(); i++)
            cout << edges->operator[](i) << ", ";
        cout << endl;
        for (int i = 0 ; i < gVector.size(); i++)
            cout << i + 1 << ": " << gVector[i] << endl;
        cout << endl;
#endif

        uint64_t cost = CostFunction(*edges, gVector);

        if (cost < minimumScore) {
            if (minimumEdges != nullptr)
                delete minimumEdges;

            cout << "Found smaller score of: " << cost << endl;

            minimumScore = cost;
            minimumEdges = edges;
        } else
            delete edges;

        if (iteration % 1000000 == 0)
            cout << iteration << endl;
    }

    auto end = high_resolution_clock::now();

    if (minimumScore != numeric_limits<uint64_t>::max() && minimumEdges != nullptr) {
        cout << "Minimum spanning tree has a cost functions of " << minimumScore << endl;
        cout << "Minimum spanning tree has the following edges: " << endl;
        for (uint64_t i = 0; i < minimumEdges->size(); i++)
            cout << (*minimumEdges)[i] << endl;

        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Time needed for the computation: " << duration.count() << " ms";
    } else {
        cout << "Finding minimum spanning tree failed.";
    }

    delete minimumEdges;
    delete resultVector;

    return 0;
}

#endif //HOMEWORK_3_H