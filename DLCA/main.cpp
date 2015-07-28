#include "dlca.h"
#include "dlca_2d.h"
#include "dlca_3d.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "uf_forest.h"

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

const int L_MIN = 3, L_MAX = 10000;
const int N_MIN = 1;
const int buffer_size = 256;

int main(int argc, char *argv[]) {
    if (argc != 5 && argc != 6) {
        cout << "Usage: " << argv[0] << " [dimension] [L] [N] [output_filename] [*snapshot_period]" << endl;
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

    int snapshot_period = 0;
    if (argc == 6) {
        snapshot_period = atoi(argv[5]);
    }

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
    
    for (;;) {
        int num_clusters = p_dlca->get_num_clusters();
        int counter = p_dlca->get_counter();
#ifdef _DEBUG
        cout << "Iter " << counter << ": " << num_clusters << " clusters" << endl;
        //p_dlca->visualize();
#endif
        if (snapshot_period > 0 && counter % snapshot_period == 0) {
            char snapshot_filename[buffer_size];
            sprintf(snapshot_filename, "%s.I%07d.txt", output_filename_, counter);
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
        } else {
            break;
        }
    }

    cout << "Total number of iterations: " << p_dlca->get_counter() << endl;
    cout << "Writing result..." << endl;

    char output_filename[buffer_size];
    if (snapshot_period > 0) {
        sprintf(output_filename, "%s.I%07d.FINAL.txt", output_filename_, p_dlca->get_counter());
    } else {
        sprintf(output_filename, "%s.txt", output_filename_);
    }
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
