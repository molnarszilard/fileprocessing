#!bin/bash
builddir=$PWD'/../build/'
input_dir=/media/rambo/ssd2/Szilard/toffilter_isaac/pcd_vox/
output_dir=/media/rambo/ssd2/Szilard/toffilter_isaac/depth_vox/
camera="isaac" #pico, nyu, kitti, isaac
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
    ./pcd2depth $input_dir $output_dir $filename $camera
done