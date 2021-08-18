#!bin/bash

builddir=$PWD'/../build/'
# basedir=/media/rambo/ssd2/Szilard/c24/
# basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
# basedir=/media/rambo/ssd2/Szilard/nyu_tofnest/
basedir=/media/rambo/ssd2/Szilard/lenssen_tofnest/
default_dir=$basedir'noise/depth/' # path to input file directory
default_iext=.png # find the files with these extension
default_od=$basedir'noise/depth_n02/' # 
default_sigma=20 #mm

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