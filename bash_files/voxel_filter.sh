#!bin/bash
builddir=$PWD'/../build/'
input_dir=/media/rambo/ssd2/Szilard/c24/640x480/pcd/
output_dir=/media/rambo/ssd2/Szilard/c24/640x480/pcdv/


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
    ./voxel_filter $input_dir $output_dir $filename 0.001f 50 0.5
done