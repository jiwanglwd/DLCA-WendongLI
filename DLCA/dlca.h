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
#include <vector>
#include <list>
#include "uf_forest.h"

using std::ostream;
using std::vector;
using std::list;

typedef int Coordinate;

// Particle id
typedef int Pid;

// Label of a cluster, a representative pid
typedef int Label;

// Doubly linked list of pid, to do splicing in O(1) time
typedef list<Pid> Cluster;

const Pid EMPTY = -1;

// Abstract base class for DLCA on square lattice of any dimension
class Dlca {
public:
    explicit Dlca(int N, int num_grid);
    virtual ~Dlca();

    virtual void visualize() const;

    // Number of particles. 0 <= pid < N
    const int N;
    
    // Return the number of iterations
    int get_counter() const;

    // Evolve to the next time step
    void evolve();

    // Return the number of clusters left
    int get_num_clusters() const;

	int get_clusters_label(Pid pid) const;

	friend ostream &operator<<(ostream &os, const Dlca &Dlca);



protected:
    // Grid id
    typedef int Gid;

    const int num_grid_;

    // grids_[gid] is the pid at gid, or EMPTY, if not present.
    Pid *grids_;

    // Union-find data structure
    // To find the label of the cluster containing any pid in almost O(1) time
    UFForest uf_forest_;

    // Labels of distinct clusters
    // Random access of vector gives the best performance when picking a cluster at random.
    vector<Label> labels_;

    // clusters_[pid] is a cluster with label pid.
    // The label should be an element in labels_.
    Cluster *clusters_;

    Label unite_and_splice(Label label_a, Label label_b);

private:
    // Count the number of iterations
    int counter_;

    // Diffuse the specified cluster and update cluster_set_
    virtual void diffuse_(Label label) = 0;

	// Print the representation of pid to an output stream, without newline
	virtual void print_particle(ostream &os, Pid pid) const = 0;

};
