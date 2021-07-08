#!bin/bash

builddir=$PWD'/../build/'
default_dir=/media/rambo/ssd2/Szilard/c24/640x480/depth2/ # path to input file directory
default_div=10

if [[ ! -z "$1" ]] 
then 
    default_dir=$1
fi
cd $default_dir

for filename in *.png; do
    cd $builddir
    # input directory, input filename, output directory, output extention, sigma
    ./isaac_normalize_depth $default_dir$filename $default_div
done