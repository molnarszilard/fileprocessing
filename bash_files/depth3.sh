#!bin/bash
builddir=$PWD'/../build/'
ddir=/media/rambo/ssd2/Szilard/c24/640x480/aug/depth/
d3dir=/media/rambo/ssd2/Szilard/c24/640x480/aug/depth3/


if [[ ! -z "$1" ]] 
then 
    ddir=$1
    if [[ ! -z "$2" ]] 
    then 
        d3dir=$2
    fi
fi
cd $ddir

for filename in *.png; do
    cd $builddir
    # depth directory, mask directory, filename
    ./depth3 $ddir $d3dir $filename
done