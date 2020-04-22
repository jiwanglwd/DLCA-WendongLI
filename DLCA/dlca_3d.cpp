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

#include "rand.h"
#include "dlca_3d.h"
#include <forward_list>
#include <algorithm>

using std::uniform_int_distribution;
using std::cout;
using std::endl;
using std::find;
using std::forward_list;

int Dlca3D::get_num_grid_from_L(int L) {
    return L * L * L;
}

const int Dlca3D::neighbors[] = {-1, 0, 1};

Dlca3D::Dlca3D(int L, int N) :
    Dlca(N, get_num_grid_from_L(L)),
    L(L),
    x_(new int[N]),
    y_(new int[N]),
    z_(new int[N]) {

    for (Coordinate x = 0; x < L; ++x) {
        for (Coordinate y = 0; y < L; ++y) {
            for (Coordinate z = 0; z < L; ++z) {
                Pid pid = grids(x, y, z);
                if (pid != EMPTY) {
                    x_[pid] = x;
                    y_[pid] = y;
                    z_[pid] = z;
                }
            }
        }
    }

    bool *removed_arr = new bool[N]();

    for (Pid pid = 0; pid < N; ++pid) {
        for (int dx : neighbors) {
            for (int dy : neighbors) {
                for (int dz : neighbors) {
                    if (dx == 0 && dy == 0 && dz == 0) continue;
                    Pid pid_neighbor = grids(
                        periodic(x_[pid] + dx),
                        periodic(y_[pid] + dy),
                        periodic(z_[pid] + dz)
                    );
                    if (pid_neighbor == EMPTY) continue;
                    Label label = uf_forest_.find(pid);
                    Label label_neighbor = uf_forest_.find(pid_neighbor);
                    if (label != label_neighbor) {
                        removed_arr[unite_and_splice(label, label_neighbor)] = true;
                    }
                }
            }
        }
    }
    for (Label label = 0; label < N; ++label) {
        if (!removed_arr[label]) labels_.push_back(label);
    }
    delete[] removed_arr;
}

Dlca3D::~Dlca3D() {
    delete[] x_;
    delete[] y_;
    delete[] z_;
}

void Dlca3D::diffuse_(Label label) {

    // Choose a diffuse direction at random (26 choices)
    int vx, vy, vz;
    // Rejection sampling
    uniform_int_distribution<int> int_distro(-1, 1);
    do {
        vx = int_distro(rand_engine);
        vy = int_distro(rand_engine);
        vz = int_distro(rand_engine);
    } while (vx == 0 && vy == 0 && vz == 0);
#ifdef _DEBUG
	cout << "Diffuse " << vx << ' ' << vy << ' ' << vz << endl;
#endif
    forward_list<Pid> pid_list_to_join_lhs, pid_list_to_join_rhs;
    const Cluster &cluster = clusters_[label];
    if (cluster.size() < N / 2) {
        //remove current cluster
		for (Pid pid : cluster) {
            grids(x_[pid], y_[pid], z_[pid]) = EMPTY;
        }
		//wendongLI adjust the speed vector, collide will lead to inversed direction
		//x
		for (Pid pid : cluster) {
			if (x_[pid] + vx < 0 || x_[pid] + vx >= L) {
				vx = -vx;
				break;
			}
			else {
				vx = vx;
			}
		}
		//y
		for (Pid pid : cluster) {
			if (y_[pid] + vy < 0 || y_[pid] + vy >= L) {
				vy = -vy;
				break;
			}
			else {
				vy = vy;
			}
		}
		//z
		for (Pid pid : cluster) {
			if (z_[pid] + vz < 0 || z_[pid] + vz >= L) {
				vz = -vz;
				break;
			}
			else {
				vz = vz;
			}
		}
		//end wendongli
		//arrange moved cluster
        for (Pid pid : cluster) {
            grids(
                x_[pid] = periodic(x_[pid] + vx),
                y_[pid] = periodic(y_[pid] + vy),
                z_[pid] = periodic(z_[pid] + vz)
            ) = pid;
        }
        for (Pid pid : cluster) {
            for (int dx : neighbors) {
                for (int dy : neighbors) {
                    for (int dz : neighbors) {
                        if (dx == 0 && dy == 0 && dz == 0) continue;
                        Pid pid_neighbor = grids(
                            periodic(x_[pid] + dx),
                            periodic(y_[pid] + dy),
                            periodic(z_[pid] + dz)
                        );
                        if (pid_neighbor == EMPTY) continue;
                        Label label_neighbor = uf_forest_.find(pid_neighbor);
                        if (label == label_neighbor) continue;
                        pid_list_to_join_lhs.push_front(pid);
                        pid_list_to_join_rhs.push_front(pid_neighbor);
                    }
                }
            }
        }
    } else {
        for (Label label_other : labels_) {
            if (label_other == label) continue;
            for (Pid pid : clusters_[label_other]) {
                grids(x_[pid], y_[pid], z_[pid]) = EMPTY;
            }
			//wendongLI adjust the speed vector, collide will lead to inversed direction
			//x
			for (Pid pid : clusters_[label_other]) {
				if (x_[pid] - vx < 0 || x_[pid] - vx >= L) {
					vx = -vx;
					break;
				}
				else {
					vx = vx;
				}
			}
			//y
			for (Pid pid : clusters_[label_other]) {
				if (y_[pid] - vy < 0 || y_[pid] - vy >= L) {
					vy = -vy;
					break;
				}
				else {
					vy = vy;
				}
			}
			//z
			for (Pid pid : clusters_[label_other]) {
				if (z_[pid] - vz < 0 || z_[pid] - vz >= L) {
					vz = -vz;
					break;
				}
				else {
					vz = vz;
				}
			}
			//end wendongli
            for (Pid pid : clusters_[label_other]) {
                grids(
                    x_[pid] = periodic(x_[pid] - vx),
                    y_[pid] = periodic(y_[pid] - vy),
                    z_[pid] = periodic(z_[pid] - vz)
                ) = pid;
            }
			offset_x_ += vx;
			offset_y_ += vy;
			offset_z_ += vz;
        }
        for (Label label_other : labels_) {
            if (label_other == label) continue;
            for (Pid pid : clusters_[label_other]) {
                for (int dx : neighbors) {
                    for (int dy : neighbors) {
                        for (int dz : neighbors) {
                            if (dx == 0 && dy == 0 && dz == 0) continue;
                            Pid pid_neighbor = grids(
                                periodic(x_[pid] + dx),
                                periodic(y_[pid] + dy),
                                periodic(z_[pid] + dz)
                            );
                            if (pid_neighbor == EMPTY) continue;
                            Label label_neighbor = uf_forest_.find(pid_neighbor);
                            if (label_neighbor == label) {
                                pid_list_to_join_lhs.push_front(pid);
                                pid_list_to_join_rhs.push_front(pid_neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
    while (!pid_list_to_join_lhs.empty()) {
        Pid pid_lhs = pid_list_to_join_lhs.front();
        Pid pid_rhs = pid_list_to_join_rhs.front();
        Label label_lhs = uf_forest_.find(pid_lhs);
        Label label_rhs = uf_forest_.find(pid_rhs);
        if (label_lhs != label_rhs) {
            int label_to_remove = unite_and_splice(label_lhs, label_rhs);
            *find(labels_.begin(), labels_.end(), label_to_remove) = labels_.back();
            labels_.pop_back();
        }
        pid_list_to_join_lhs.pop_front();
        pid_list_to_join_rhs.pop_front();
    }
}

Coordinate Dlca3D::periodic(Coordinate c) const {
    // Assume x and y are not too far away for the grid
	// WendongLI: Particle collide on the boundaries
    if (c < 0) {
        return -c;
    } else if (c >= L) {
        return 2*(L-1)-c;
    } else {
        return c;
    }
}

Pid &Dlca3D::grids(Coordinate x, Coordinate y, Coordinate z) const {
    return grids_[x * L * L + y * L + z];
}

void Dlca3D::print_particle(ostream &os, Pid pid) const {
    os << periodic(x_[pid] + offset_x_) << ' '
       << periodic(y_[pid] + offset_y_) << ' '
       << periodic(z_[pid] + offset_z_);
}
