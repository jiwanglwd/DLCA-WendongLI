# DLCA

A C++ implementation of the **diffusion-limited cluster-cluster aggregation** (DLCA), an algorithm to generate aerogel structures.

# Usage

1. Parameters

    - ```[D]``` Dimension of the grid.

    - ```[L]``` Size of the grid. E.g. ```L``` x ```L``` for ```D``` = 2.

    - ```[N]``` Number of particles in the grid.

    - ```[F]``` The output file name, without ```.txt``` suffix. E.g. ```output/D2.L10.N10```.

    - ```[P]``` The period for snapshots.

    - ```[I]``` The number of iterations.

    - ```[M]``` The marker size of plots.

    - ```[ELEV]``` The elevation angle.

    - ```[AZIM]``` The azimuthal angle.

2. Single run

    - Windows

        - 32-bit

            ```
            exe\win32\DLCA.exe [D] [L] [N] [F]
            ```

        - 64-bit

            ```
            exe\win64\DLCA.exe [D] [L] [N] [F]
            ```
    - Linux

        ```
        # C++11 support required
        make
        ```

        ```
        bin/dlca [D] [L] [N] [F]
        ```

3. Plotting single run ([Python](https://www.python.org/), [NumPy](http://www.numpy.org/), and [matplotlib](http://matplotlib.org/) required)

    - 2D

        ```
        python plot_2d.py [L] [INPUT] [OUTPUT] [M]
        ```

    - 3D

        ```
        python plot_3d.py [L] [INPUT] [OUTPUT] [M] [ELEV] [AZIM]
        ```

4. Creating GIF ([ImageMagick](http://www.imagemagick.org/) required)

    - Windows

        ```
        create_demo.bat [D] [L] [N] [P] [M]
        ```

    - Linux

        ```
        create_demo.sh [D] [L] [N] [P] [M]
        ```

# GIF demo

- [D2.L128.N2000](demo/D2.L128.N2000)

    ![D2.L128.N2000](demo/D2.L128.N2000/animation.gif)

- [D2.L1024.N50000](demo/D2.L1024.N50000)

    ![D2.L1024.N50000](demo/D2.L1024.N50000/animation.gif)

- [D3.L100.N6000](demo/D3.L100.N6000)

    ![D3.L100.N6000](demo/D3.L100.N6000/animation.gif)

# Benchmark



# Related info

- [Computer Simulations of Cluster-Cluster Aggregation](http://dx.doi.org/10.1039/DC9878300125)

- [Formation of Fractal Clusters and Networks by Irreversible Diffusion-Limited Aggregation](http://dx.doi.org/10.1103/PhysRevLett.51.1119)

- [Mechanical structure-property relationship of aerogels](http://dx.doi.org/10.1016/S0022-3093(00)00288-X)

- [Scaling of Kinetically Growing Clusters](http://dx.doi.org/10.1103/PhysRevLett.51.1123)

# License

This software is licensed under the [GNU General Public License](LICENSE).
