#!bin/bash

builddir=$PWD'/../build/'
basedir=/mnt/ssd1/datasets/c24/evaluation/
default_dir=$basedir'depth/' # path to input file directory
default_iext=.png # find the files with these extension
default_od=$basedir'noise/depth_n10/' # 
default_sigma=100 #mm

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