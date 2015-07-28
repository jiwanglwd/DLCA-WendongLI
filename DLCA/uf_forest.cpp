#include "uf_forest.h"
#include <iostream>
#include <iomanip>

using std::setw;
using std::endl;

void UFForest::unite(Element i, Element j) {
    Element root_i = find(i);
    Element root_j = find(j);
    if (root_i != root_j) {
        unite_roots(root_i, root_j);
    }
}

bool UFForest::unite_roots(Element root_i, Element root_j) {
    // Union by rank
    if (rank_[root_i] < rank_[root_j]) {
        parent_[root_i] = root_j;
        return false;
    } else {
        parent_[root_j] = root_i;
        if (rank_[root_i] == rank_[root_j]) {
            ++rank_[root_i];
        }
        return true;
    }
}

Element UFForest::find(Element i) {
    // Path compression using recursion
    return parent_[i] == i ? i : parent_[i] = find(parent_[i]);
}

bool UFForest::is_connected(Element i, Element j) {
    return find(i) == find(j);
}

UFForest::UFForest(int N):
        N(N),
        parent_(new Element[N]),
        rank_(new Rank[N]) {
    for (Element i = 0; i < N; ++i) {
        parent_[i] = i;
        rank_[i] = 0;
    }
}

UFForest::~UFForest() {
    delete [] parent_;
    delete [] rank_;
}

ostream &operator<<(ostream &os, const UFForest &ds) {
    const int width = 6;
    os << setw(width) << "id";
    for (Element i = 0; i < ds.N; ++i) {
        os << setw(width) << i;
    }
    os << '\n' << setw(width) << "parent";
    for (Element i = 0; i < ds.N; ++i) {
        os << setw(width) << ds.parent_[i];
    }
    os << '\n' << setw(width) << "rank";
    for (Element i = 0; i < ds.N; ++i) {
        os << setw(width) << ds.rank_[i];
    }
    os << endl;
    return os;
}
