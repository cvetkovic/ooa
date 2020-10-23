//
// Created by jugos000 on 23-Oct-20.
//

#ifndef TESTGROUND_PERMUTATIONS_H
#define TESTGROUND_PERMUTATIONS_H

class Permutations {
private:
    uint64_t startFrom;
    uint64_t length;
    uint64_t* number;

public:
    Permutations(uint64_t startFrom, uint64_t length) : startFrom(startFrom), length(length) {
        number = new int[length];
    }

    ~Permutations() {
        delete[] number;
    }

    const uint64_t* const GetCurrent() const {

    }

    void Next();

    bool HasNext();

    int GetAsNumber();
};

void Permutations::Next() {

}

int Permutations::GetAsNumber() {
    return 0;
}

#endif //TESTGROUND_PERMUTATIONS_H
