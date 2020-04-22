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
#include "dlca_2d.h"
#include "dlca_3d.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "uf_forest.h"
#include <ctime>
#include <string>
#include <cmath>

using namespace std;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;

void test_uf_forest() {
    UFForest uf_forest(6);
    cout << uf_forest << endl;
    uf_forest.unite(1, 2);
    cout << uf_forest << endl;
    uf_forest.unite(4, 0);
    cout << uf_forest << endl;
    uf_forest.unite(2, 5);
    cout << uf_forest << endl;
    uf_forest.unite(0, 1);
    cout << uf_forest << endl;
    uf_forest.find(2);
    cout << uf_forest << endl;
    uf_forest.unite(5, 3);
    cout << uf_forest << endl;
}

string getTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime(&timep));
    return tmp;
}

const int L_MIN = 3, L_MAX = 10000;
const int N_MIN = 1;
const int buffer_size = 1024;
const int snapshot_MAX = 1024;
const string current_time = getTime();
const char *chartime = current_time.c_str();



int main(int argc, char *argv[]) {	
	if (argc != 5 && argc != 7) {
        cout << "Usage: " << argv[0] << " [dimension] [L] [N] [output_filename] [*snapshot_style] [*snapshot_time] " << endl;
        return EXIT_SUCCESS;
    }
    int dimension = atoi(argv[1]);
    int L = atoi(argv[2]);
    if (L < L_MIN || L > L_MAX) {
        cerr << "Required: " << L_MIN << " <= L <= " << L_MAX << endl;
        return EXIT_FAILURE;
    }
    int N = atoi(argv[3]);
    if (N < N_MIN) {
        cerr << "Required: " << "N >= " << N_MIN << endl;
        return EXIT_FAILURE;
    }
    char *output_filename_ = argv[4];

    int snapshot_time = 0;
	int snapshot_style = 0;
    if (argc == 7) {
		snapshot_style = atoi(argv[5]);//0=cluster based, 1=counter based
        snapshot_time = atoi(argv[6]);
		if (snapshot_style != 0 && snapshot_style != 1) {
			cerr << "Required: " << "snapshot style = 0 (cluster based) or 1 (counter based)." << endl;
			cerr << "Exiting..." << endl;
			return EXIT_FAILURE;
		}
		else if (snapshot_time<=0){
			cerr << "Required: " << "snapshot time >0." << endl;
			cerr << "Exiting..." << endl;
			return EXIT_FAILURE;
		}
    }
	int snapshot_moments[snapshot_MAX] = { 0 };
	int current_snapshot = 0;

    Dlca *p_dlca;

    // Choose the appropriate concrete subclass of Dlca, based on dimension
    if (dimension == 2) {
        int num_grid = Dlca2D::get_num_grid_from_L(L);
        if (N > num_grid) {
            cerr << "Required: " << "N <= " << num_grid << endl;
            return EXIT_FAILURE;
        }
        p_dlca = new Dlca2D(L, N);
    } else if (dimension == 3) {
        int num_grid = Dlca3D::get_num_grid_from_L(L);
        if (N > num_grid) {
            cerr << "Required: " << "N <= " << num_grid << endl;
            return EXIT_FAILURE;
        }
        p_dlca = new Dlca3D(L, N);
    } else {
        cerr << "Cannot handle dimension = " << dimension << endl;
        cerr << "Exiting..." << endl;
        return EXIT_FAILURE;
    }
	int num_clusters_first = p_dlca->get_num_clusters();
	int num_clusters_last = 1;
	if (snapshot_style == 0) {
		if (snapshot_time <= 0) {
			cerr << "Required: " << "snapshot time >0." << endl;
			cerr << "Exiting..." << endl;
			return EXIT_FAILURE;
		}
		else {
			if (snapshot_time > snapshot_MAX) {
				cerr << "Required: " << "snapshot time <= " << snapshot_MAX << endl;
				cerr << "Exiting..." << endl;
				return EXIT_FAILURE;
			}
			double snapshot_interval = (double(num_clusters_first) - double(num_clusters_last)) / (double(snapshot_time) - 1.0);
			for (int i = 0; i < snapshot_time - 2; i++) {
				snapshot_moments[i] = round(num_clusters_first - (i + 1)*snapshot_interval);
			}
		}
	}
	else if (snapshot_style == 1) {
		snapshot_time = snapshot_time;
	}
	else {
		cerr << "Required: " << "snapshot style = 0 or 1"<<endl;
		cerr << "Exiting..." << endl;
		return EXIT_FAILURE;
	}

    for (;;) {
        int num_clusters = p_dlca->get_num_clusters();
        int counter = p_dlca->get_counter();		
#ifdef _DEBUG
        cout << "Iter " << counter << ": " << num_clusters << " clusters" << endl;
		cout << "Before:" << endl;
        p_dlca->visualize();
#endif
		//first smapshot for initial distribution of particles
		if (counter == 0) {
			char snapshot_filename[buffer_size];
			
			sprintf(snapshot_filename, "%s_%s_D%d_L%d_N%d_C%d_I%d_INITIAL.csv",chartime, output_filename_, dimension, L, N, num_clusters,counter);
			ofstream ofs_snapshot(snapshot_filename);
			if (!ofs_snapshot) {
				cerr << "Failed to open file " << snapshot_filename << endl;
				return EXIT_FAILURE;
			}
			ofs_snapshot << *p_dlca;
			ofs_snapshot.close();
		}
		// snapshot judged by clusters:
		

		if (snapshot_style == 0 && num_clusters==snapshot_moments[current_snapshot]) {
			char snapshot_filename[buffer_size];
			sprintf(snapshot_filename, "%s_%s_D%d_L%d_N%d_C%d_I%d.csv", chartime, output_filename_, dimension, L, N, num_clusters, counter);
			ofstream ofs_snapshot(snapshot_filename);
			if (!ofs_snapshot) {
				cerr << "Failed to open file " << snapshot_filename << endl;
				return EXIT_FAILURE;
			}
			ofs_snapshot << *p_dlca;
			ofs_snapshot.close();
			current_snapshot++;
		}
		//snapshot judged by iteration counts:
		else if (snapshot_style == 1 && counter % snapshot_time == 0 && counter >0) {
            char snapshot_filename[buffer_size];
            sprintf(snapshot_filename, "%s_%s_D%d_L%d_N%d_C%d_I%d.csv",chartime, output_filename_, dimension, L, N, num_clusters, counter);
            ofstream ofs_snapshot(snapshot_filename);
            if (!ofs_snapshot) {
                cerr << "Failed to open file " << snapshot_filename << endl;
                return EXIT_FAILURE;
            }
            ofs_snapshot << *p_dlca;
            ofs_snapshot.close();
        }
        if (num_clusters > 1) {
            p_dlca->evolve();
#ifdef _DEBUG
			cout << "After:" << endl;
			p_dlca->visualize();
			cout << "Press any Key..." << endl;
			cin.get();
#endif
        } else {
            break;
        }
    }

    cout << "Total number of iterations: " << p_dlca->get_counter() << endl;
	cout << "Writing result..." << endl;

    char output_filename[buffer_size];
	sprintf(output_filename, "%s_%s_D%d_L%d_N%d_C%d_I%d_FINAL.csv",chartime, output_filename_, dimension, L, N, p_dlca->get_num_clusters(), p_dlca->get_counter());
    ofstream ofs_result(output_filename);
    if (!ofs_result) {
        cerr << "Failed to open file " << output_filename << endl;
        return EXIT_FAILURE;
    }
    ofs_result << *p_dlca;
    ofs_result.close();

    delete p_dlca;
    return EXIT_SUCCESS;
}
