#!bin/bash
builddir=$PWD'/../build/'
basedir=/media/rambo/ssd2/Szilard/pico_tofnest/
input_dir=$basedir'pcdpred_delta/' # path to input file directory
output_dir=$basedir'pcdrgb/' # path to input file directory
camera="pico" #pico, nyu, kitti, isaac
if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *.pcd; do
    cd $builddir
    # path, height, width, cameratype
    ./cloud2rgbimage $input_dir $output_dir $filename $camera
done