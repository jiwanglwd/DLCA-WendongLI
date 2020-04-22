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

#include "dlca.h"
#include "rand.h"
#include <algorithm>
#include <iomanip>

using std::shuffle;
using std::cout;
using std::endl;
using std::cin;
using std::uniform_int_distribution;

Dlca::Dlca(int N, int num_grid):
        N(N),
        num_grid_(num_grid),
        grids_(new int[num_grid]),
        uf_forest_(N), 
        clusters_(new Cluster[N]),
        counter_(0) {
    // Initially each particle is one cluster.
    for (Pid pid = 0; pid < N; ++pid) {
        clusters_[pid].push_front(pid);
    }
    for (Gid gid = 0; gid < num_grid_; ++gid) {
        grids_[gid] = EMPTY;
    }
    // Shuffle the sequence of gid to randomly choose N positions
    Gid *gid_seq = new Gid[num_grid_];
    for (Gid gid = 0; gid < num_grid_; ++gid) {
        gid_seq[gid] = gid;
    }
    shuffle(gid_seq, gid_seq + num_grid_, rand_engine);
    for (Pid pid = 0; pid < N; ++pid) {
        // Place pid at gid gid_seq[pid]
        grids_[gid_seq[pid]] = pid;
    }
    delete[] gid_seq;
}

Dlca::~Dlca() {
    delete [] grids_;
    delete[] clusters_;
}

void Dlca::visualize() const {
}

int Dlca::get_counter() const {
    return counter_;
}

void Dlca::evolve() {
    ++counter_;
    //if (get_num_clusters() < 30 && counter_ % 100 == 0) {
    //    cout << "Iter " << counter_ << ": " << get_num_clusters() << " clusters" << std::endl;
    //    for (int i = 0; i < get_num_clusters(); ++i) {
    //        cout << clusters_[labels_[i]].size() << std::endl;
    //    }
    //}
    // Choose a cluster at random
    const int label = labels_[
        uniform_int_distribution<int>(
            0, get_num_clusters() - 1
        )(rand_engine)
    ];
#ifdef _DEBUG
    cout << "Label " << label << ' ';
#endif
    diffuse_(label);
}

int Dlca::get_num_clusters() const {
    return labels_.size();
}

ostream &operator<<(ostream &os, const Dlca &dlca) {
    for (Pid pid = 0; pid < dlca.N; ++pid) {
        dlca.print_particle(os, pid);
        os << '\n';
    }
    return os;
}

Label Dlca::unite_and_splice(Label label_a, Label label_b) {
    // Either label_a or label_b will be removed.
    Label label_to_remove;
    if (uf_forest_.unite_roots(label_a, label_b)) {
        // label_a remained
        clusters_[label_a].splice(clusters_[label_a].end(), clusters_[label_b]);
        label_to_remove = label_b;
    } else {
        // label_b remained
        clusters_[label_b].splice(clusters_[label_b].end(), clusters_[label_a]);
        label_to_remove = label_a;
    }
    return label_to_remove;
}
