#!/bin/bash

run () {
    DEMO_DIR=demo/D$D.L$L.N$N
    mkdir -p $DEMO_DIR
    BIN=./bin/dlca
    
    if [ -x $BIN ]; then
        echo Running...
        $BIN $D $L $N $DEMO_DIR/D$D.L$L.N$N $P
    else
        echo $BIN not found!
        exit 1
    fi
    
    echo Creating visualization...
    
    for FILE in $DEMO_DIR/*.txt
    do
        BASENAME=$(basename $FILE .txt)
        echo Visualizing $BASENAME.txt
        python visualize.py $D $L $FILE $DEMO_DIR/$BASENAME.png $M
    done
    
    echo Creating gif...
    
    pushd $DEMO_DIR>/dev/null
    # ImageMagick required
    convert -delay 50 +page *.png -loop 1 animation.gif
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
