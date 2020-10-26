//
// Created by jugos000 on 26-Oct-20.
//

#ifndef HOMEWORK_3_VARIATIONS_H
#define HOMEWORK_3_VARIATIONS_H

class VariationGenerator {

private:
    bool variationExhausted = false;
    const uint64_t n, k;
    int q;
    uint64_t *generatedNumber;

public:
    VariationGenerator(const int n, const int k) : n(n), k(k), q(0) {
        generatedNumber = new uint64_t[k];
        for (int i = 0; i < k; i++)
            generatedNumber[i] = 0;
    }

    ~VariationGenerator() {
        delete[] generatedNumber;
    }

    bool HasNext() const {
        return !variationExhausted;
    }

    const uint64_t *NextVariation(uint64_t *resultVector) {
        for (int i = 0; i < k; i++)
            resultVector[i] = generatedNumber[i];

        q = k - 1;
        while (q >= 0) {
            generatedNumber[q]++;
            if (generatedNumber[q] == n) {
                generatedNumber[q] = 0;
                q--;
            } else
                break;
        }

        if (q < 0)
            variationExhausted = true;

        return resultVector;
    }

};

#endif //HOMEWORK_3_VARIATIONS_H
