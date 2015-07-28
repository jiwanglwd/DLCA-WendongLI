// Copyright (C) 2015 Zhang NS

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
