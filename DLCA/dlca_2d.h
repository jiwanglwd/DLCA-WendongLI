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
