#!/bin/bash

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

GIF_DELAY_D2=50
GIF_DELAY_D3=20
GIF_LOOP=1
GIF_FILENAME=animation

# Frame per state
D3_FPS=5
D3_ELEV=30
D3_AZIM_0=0
# Increment of AZIM per frame
D3_AZIM_FDELTA=1

run () {
    DEMO_DIR=demo/D$D.L$L.N$N
    mkdir -p $DEMO_DIR
    BIN=./bin/dlca
    
    if [ -x $BIN ]; then
        echo Running DLCA...
        $BIN $D $L $N $DEMO_DIR/D$D.L$L.N$N $P
    else
        echo $BIN not found!
        exit 1
    fi
    
    echo Creating visualization...
    
    if [ $D == 2 ]; then
        GIF_DELAY=$GIF_DELAY_D2
        for FILE in $DEMO_DIR/*.txt
        do
            BASENAME=$(basename $FILE .txt)
            echo $BASENAME.txt
            python plot_2d.py $L $FILE $DEMO_DIR/$BASENAME.png $M
        done
    elif [ $D == 3 ]; then
        GIF_DELAY=$GIF_DELAY_D3
        AZIM=$D3_AZIM_0
        for FILE in $DEMO_DIR/*.txt
        do
            BASENAME=$(basename $FILE .txt)
            echo $BASENAME.txt
            for i in $(seq 1 $D3_FPS)
            do
                python plot_3d.py $L $FILE $DEMO_DIR/$BASENAME.$(printf %03d $i).png $M $D3_ELEV $AZIM
                AZIM=$(($AZIM+$D3_AZIM_FDELTA))
            done
        done
    else
        echo Invalid dimension
        exit 1
    fi
    
    pushd $DEMO_DIR>/dev/null
    echo Creating GIF...
    # ImageMagick required
    convert -delay $GIF_DELAY +page *.png -loop $GIF_LOOP $GIF_FILENAME.gif
    popd>/dev/null
}

if [ $# == 5 ]; then
    D=$1
    L=$2
    N=$3
    P=$4
    M=$5
    echo D = $D
    echo L = $L
    echo N = $N
    echo P = $P
    echo M = $M
    run
else
    echo Usage: [D] [L] [N] [P] [M]
fi
