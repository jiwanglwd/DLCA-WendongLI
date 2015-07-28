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

// Collision: NN + NNN
// Diffusion: NN + NNN
class Dlca3D : public Dlca {
public:
    static int get_num_grid_from_L(int L);

    explicit Dlca3D(int L, int N);
    ~Dlca3D();

private:
    static const int neighbors[];

    const int L;

    Coordinate *x_, *y_, *z_;

    void diffuse_(Label label) override;

    Coordinate periodic(Coordinate c) const;

    Pid &grids(Coordinate x, Coordinate y, Coordinate z) const;

    void print_particle(ostream &os, Pid pid) const override;

    int offset_x_ = 0;
    int offset_y_ = 0;
    int offset_z_ = 0;

};
