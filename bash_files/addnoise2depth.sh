#!bin/bash

builddir=$PWD'/../build/'
default_dir=/media/rambo/ssd2/Szilard/c24/640x480/aug/depth/ # path to input file directory
default_iext=.png # find the files with these extension
default_od=/media/rambo/ssd2/Szilard/c24/640x480/aug/ndepth/ # 
default_sigma=1000

if [[ ! -z "$1" ]] 
then 
    default_dir=$1
fi
cd $default_dir

for filename in *.png; do
    cd $builddir
    # input directory, input filename, output directory, output extention, sigma
    ./addnoise2depth $default_dir $filename $default_od $default_sigma
done