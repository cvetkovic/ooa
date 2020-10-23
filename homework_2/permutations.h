//
// Created by jugos000 on 23-Oct-20.
//

#ifndef TESTGROUND_PERMUTATIONS_H
#define TESTGROUND_PERMUTATIONS_H

#include <iostream>

using namespace std;

class Permutations {
private:
    uint64_t length;
    uint64_t *number;

    bool hasNext = true;

public:
    Permutations(uint64_t length) : length(length) {
        number = new uint64_t[length];

        for (uint64_t i = 0; i < length; i++)
            number[i] = i;
    }

    ~Permutations() {
        delete[] number;
    }

    const uint64_t *const GetCurrent() const {
        return number;
    }

    void Next();

    bool HasNext();

    friend ostream &operator<<(ostream &os, const Permutations &permutations) {
        for (uint64_t i = 0; i < permutations.length; i++)
            os << (char) (permutations.number[i] + 'A') << " ";

        return os;
    }

    uint64_t *GetCurrentAsPtr(uint64_t *ptr);
};

void Permutations::Next() {
    int s;
    uint64_t *first = &number[0];
    uint64_t *last = &number[length - 1];
    uint64_t *k = last - 1;
    uint64_t *l = last;

    while (k > first) {
        if (*k < *(k + 1))
            break;

        k--;
    }

    if (*k > *(k + 1)) {
        hasNext = false;
        return;
    }

    while (l > k) {
        if (*l > *k)
            break;
        l--;
    }

    s = *k;
    *k = *l;
    *l = s;

    first = k + 1;
    while (first < last) {
        s = *first;
        *first = *last;
        *last = s;

        first++;
        last--;
    }
}

bool Permutations::HasNext() {
    return hasNext;
}

uint64_t *Permutations::GetCurrentAsPtr(uint64_t *ptr) {
    if (ptr == nullptr)
        ptr = new uint64_t[length];

    for (uint64_t i = 0; i < length; i++)
        ptr[i] = number[i];

    return ptr;
}

#endif //TESTGROUND_PERMUTATIONS_H
