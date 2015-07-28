# Copyright (C) 2015 Zhang NS

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Python 3
import sys
import os.path
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def get_filename(path):
    return os.path.splitext(os.path.basename(path))[0]

def main():
    if len(sys.argv) != 7:
        print('Usage: [L] [INPUT] [OUTPUT] [M] [ELEV] [AZIM]')
        return
    L, INPUT, OUTPUT, MARKERSIZE, ELEV, AZIM = sys.argv[1:]
    L = int(L)
    assert L > 0
    MARKERSIZE = int(MARKERSIZE)
    assert MARKERSIZE > 0
    ELEV = float(ELEV)
    AZIM = float(AZIM)
    plot_3d(L, INPUT, OUTPUT, MARKERSIZE, ELEV, AZIM)
    
def plot_3d(L, INPUT, OUTPUT, MARKERSIZE, ELEV, AZIM):
    r = np.loadtxt(INPUT)
    fig = plt.gcf()
    ax = fig.gca(projection='3d')
    ax.view_init(elev=ELEV, azim=AZIM)
    axis_range = [-0.5, L - 0.5]
    ax.set_xlim(*axis_range)
    ax.set_ylim(*axis_range)
    ax.set_zlim(*axis_range)
    ax.set_aspect('equal')
    ax.set_title(get_filename(INPUT))
    ax.scatter(r[:,0], r[:,1], r[:,2], c='k', marker='.', s=MARKERSIZE)
    plt.tight_layout()
    plt.draw()
    plt.savefig(OUTPUT)

if __name__ == '__main__':
    main()
