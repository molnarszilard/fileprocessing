#!bin/bash
builddir=$PWD'/../build/'
input_dir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/pcdnoise_n05/
output_dir=/media/rambo/ssd2/Szilard/nyu_v2_filter/comparison/xyz_n05/

if [[ ! -z "$1" ]] 
then 
    input_dir=$1
    if [[ ! -z "$2" ]] 
    then 
        output_dir=$2
    fi
fi
cd $input_dir

for filename in *0.pcd; do
    cd $builddir
    ./pcd2xyz $input_dir $output_dir $filename
done