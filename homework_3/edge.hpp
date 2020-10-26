//
// Created by jugos000 on 26-Oct-20.
//

#ifndef HOMEWORK_3_EDGE_HPP
#define HOMEWORK_3_EDGE_HPP

#include <iostream>
using namespace std;

class SpanningTreeEdge {

public:
    uint64_t from, to;

public:
    SpanningTreeEdge(uint64_t from, uint64_t to) : from(from), to(to) {}

    friend ostream &operator<<(ostream &o, const SpanningTreeEdge edge) {
        o << "(" << edge.from + 1 << " - " << edge.to + 1 << ")";

        return o;
    }

};

#endif //HOMEWORK_3_EDGE_HPP
