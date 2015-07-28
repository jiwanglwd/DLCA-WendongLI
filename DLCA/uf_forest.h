#pragma once
#include <iostream>

using std::ostream;

typedef int Element;

// Union-find forest (disjoint sets of elements)
class UFForest {
public:
    // Instantiate with a given number of elements
    explicit UFForest(int N);
    ~UFForest();

    // Connect two elements
    void unite(Element i, Element j);

    // Connect two roots (unchecked)
    // Return whether root_i remains a root
    bool unite_roots(Element root_i, Element root_j);

    // Get the label of the connected component containing a specific element
    Element find(Element i);

    // Whether two elements are connected
    bool is_connected(Element i, Element j);

private:
    typedef int Rank;
    const int N;

    Element *parent_;
    Rank *rank_;

    friend ostream &operator<<(ostream &os, const UFForest &ds);
};
