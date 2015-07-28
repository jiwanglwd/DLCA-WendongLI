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
