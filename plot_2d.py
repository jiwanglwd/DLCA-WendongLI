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


def get_filename(path):
    return os.path.splitext(os.path.basename(path))[0]

def main():
    if len(sys.argv) != 5:
        print('Usage: [L] [INPUT] [OUTPUT] [M]')
        return
    L, INPUT, OUTPUT, MARKERSIZE = sys.argv[1:]
    L = int(L)
    assert L > 0
    MARKERSIZE = int(MARKERSIZE)
    assert MARKERSIZE > 0
    plot_2d(L, INPUT, OUTPUT, MARKERSIZE)
    
def plot_2d(L, INPUT, OUTPUT, MARKERSIZE):
    r = np.loadtxt(INPUT)
    ax = plt.gca()
    ax.axis([-0.5, L - 0.5] * 2)
    ax.set_autoscale_on(False)
    ax.set_aspect('equal')
    ax.set_title(get_filename(INPUT))
    ax.scatter(r[:,0], r[:,1], c='k', marker='.', s=MARKERSIZE)
    plt.tight_layout()
    plt.draw()
    plt.savefig(OUTPUT)

if __name__ == '__main__':
    main()
