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
