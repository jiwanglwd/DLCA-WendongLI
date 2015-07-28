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
#include "dlca.h"
#include <iostream>

using std::ostream;

// Collision: NN + NNN
// Diffusion: NN + NNN
class Dlca2D : public Dlca {
public:
    static int get_num_grid_from_L(int L);

    // Initialize with L (grid size, unchecked) and N (number of particles, unchecked)
    explicit Dlca2D(int L, int N);
    ~Dlca2D();

    friend ostream &operator<<(ostream &os, const Dlca2D &dlca_2d);

    void visualize() const override;

private:
    static const int neighbors[];

    // L x L grid
    const int L;

    // x, y coordinates of pid, to efficiently move a cluster
    Coordinate *x_, *y_;

    void diffuse_(Label label) override;

    // Employ periodic boundary conditions
    Coordinate periodic(Coordinate c) const;

    // Friendly wrapper for the underlying array
    // grids(x, y) is the pid at Grid (x, y), or EMPTY, if not present.
    // Pairwise traversal of complexity O(N^2) is thus avoided.
    Pid &grids(Coordinate x, Coordinate y) const;

    void print_particle(ostream &os, Pid pid) const override;

    // Displacement of the reference frame
    int offset_x_ = 0;
    int offset_y_ = 0;

};
