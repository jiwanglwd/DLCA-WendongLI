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

	Element getLabel(Element i) const;

    // Whether two elements are connected
    bool is_connected(Element i, Element j);

private:
    typedef int Rank;
    const int N;

    Element *parent_;
    Rank *rank_;

    friend ostream &operator<<(ostream &os, const UFForest &ds);
};
